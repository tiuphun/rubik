#include "MessageHandler.h"
#include "../services/UserService.h"
#include "../models/header/Player.h"
#include "../models/header/Admin.h"
#include "../models/header/Room.h"
#include "../models/header/RoomParticipant.h"
#include "../models/header/GameSession.h"
#include <iostream>

using namespace std;

json MessageHandler::parseMessage(const std::string& message) {
    return json::parse(message);
}

json MessageHandler::handleMessage(const json& parsed_message, sqlite3* db, int client_socket) {
    std::string type = parsed_message["type"];
    std::string data = parsed_message["data"].dump();
    printf("Message type: %s\n", type.c_str());
    printf("Message data: %s\n", data.c_str());

    printf("Bat dau doan cua Phong\n");
    // Print initialization status of member variables
    cout << "EntityManager initialized: " << (entityManager) << endl;
    cout << "AuthRepo initialized: " << (&authRepo) << endl;
    cout << "PlayerRepo initialized: " << (&playerRepo) << endl;
    cout << "AdminRepo initialized: " << (&adminRepo) << endl;
    cout << "UserService initialized: " << (&userService) << endl;
    cout << "PlayerService initialized: " << (&playerService) << endl;
    cout << "AdminService initialized: " << (&adminService) << endl;
    cout << "RoomService initialized: " << (&roomService) << endl;
    cout << "GameService initialized: " << (&gameService) << endl;

    printf("Ket thuc doan cua Phong\n");
    // cout << "AuthRepo initialized: " << (this->authRepo) << endl;
    // cout << "PlayerRepo initialized: " << (this->playerRepo) << endl;
    // cout << "AdminRepo initialized: " << (this->adminRepo) << endl;
    // cout << "UserService initialized: " << (this->userService) << endl;
    // cout << "PlayerService initialized: " << (this->playerService) << endl;
    // cout << "AdminService initialized: " << (this->adminService) << endl;
    // cout << "RoomService initialized: " << (this->roomService) << endl;
    // cout << "GameService initialized: " << (this->gameService) << endl;

    json response_data;

    if (type == "SIGN_UP") {
        printf("Handling sign up\n");
        response_data = handleSignUp(parsed_message["data"], db);
    } else if (type == "SIGN_IN") {
        printf("Handling sign in\n");
        response_data = handleSignIn(parsed_message["data"], db, client_socket);
    } else if (type == "VIEW_ROOMS") {
        response_data = handleViewRooms(parsed_message["data"], db);
    } else if (type == "CREATE_ROOM") {
        response_data = handleCreateRoom(parsed_message["data"], db);
    } else if (type == "JOIN_ROOM") {
        response_data = handleJoinRoom(parsed_message["data"], db);
    } else if (type == "LEAVE_ROOM") {
        response_data = handleLeaveRoom(parsed_message["data"], db);
    } else if (type == "START_GAME") {
        response_data = handleStartGame(parsed_message["data"], db);
    } else if (type == "END_GAME") {
        response_data = handleEndGame(parsed_message["data"], db);
    } else if (type == "VIEW_USERS") {
        response_data = handleViewUsers(parsed_message["data"], db);
    } else if (type == "BAN_PLAYER") {
        response_data = handleBanPlayer(parsed_message["data"], db);
    } else if (type == "SPECTATE") {
        response_data = handleSpectate(parsed_message["data"], db);
    } else if (type == "UPDATE_CUBE") {
        response_data = handleCubeUpdate(parsed_message["data"], db);
    }
    return response_data;
}

json MessageHandler::handleSignUp(const json& data, sqlite3* db) {
    std::string username = data["username"];
    std::string password = data["password"];
    printf("Username: %s\n", username.c_str());
    printf("Password: %s\n", password.c_str());
    return userService.signUp(username, password);
}

json MessageHandler::handleSignIn(const json& data, sqlite3* db, int client_socket) {
    std::string username = data["username"];
    std::string password = data["password"];
    printf("Username: %s\n", username.c_str());
    printf("Password: %s\n", password.c_str());
    return userService.signIn(username, password, client_socket);
}

json MessageHandler::handleViewRooms(const json& data, sqlite3* db) {
    return playerService.viewRoomList();
}

json MessageHandler::handleCreateRoom(const json& data, sqlite3* db) {
    int player_id = data["player_id"];
    int max_players = data["max_players"];
    int max_spectators = data["max_spectators"];
    return playerService.createRoom(player_id, max_players, max_spectators);
}

json MessageHandler::handleJoinRoom(const json& data, sqlite3* db) {
    int player_id = data["player_id"];
    int room_id = data["room_id"];
    std::string participant_type_str = data["participant_type"];
    RoomParticipantStatus participant_type = stringToRoomParticipantStatus(participant_type_str);
    return playerService.joinRoom(player_id, room_id, participant_type);
}

json MessageHandler::handleLeaveRoom(const json& data, sqlite3* db) {
    int room_id = data["room_id"];
    int participant_id = data["participant_id"];
    return roomService.removeParticipant(room_id, participant_id);
}

json MessageHandler::handleStartGame(const json& data, sqlite3* db) {
    int player_id = data["player_id"];
    int room_id = data["room_id"];
    return roomService.startGameSession(player_id, room_id);
}

json MessageHandler::handleEndGame(const json& data, sqlite3* db) {
    int player_id = data["player_id"];
    int game_session_id = data["game_session_id"];
    return gameService.endGameSession(game_session_id, player_id);
}

json MessageHandler::handleViewUsers(const json& data, sqlite3* db) {
    vector<Player> players = playerRepo.getAllPlayers();
    return playerService.returnPlayersJson(players);
}

json MessageHandler::handleBanPlayer(const json& data, sqlite3* db) {
    int admin_id = data["admin_id"];
    int player_id = data["player_id"];
    return adminService.banPlayer(player_id, admin_id);
}

json MessageHandler::handleSpectate(const json& data, sqlite3* db) {
    int room_id = data["room_id"];
    int participant_id = data["participant_id"];
    return adminService.spectate(room_id, participant_id);
}

json MessageHandler::handleCubeUpdate(const json& data, sqlite3* db) {
    int player_id = data["player_id"];
    int game_session_id = data["game_session_id"];
    string new_cube_state = data["new_cube_state"];
    return gameService.updateCubeState(game_session_id, player_id, new_cube_state);
}