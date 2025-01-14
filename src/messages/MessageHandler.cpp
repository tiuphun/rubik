#include "MessageHandler.h"
#include "../services/UserService.h"
#include "../models/header/Player.h"
#include "../models/header/Admin.h"
#include "../models/header/Room.h"
#include "../models/header/RoomParticipant.h"
#include "../models/header/GameSession.h"

json MessageHandler::parseMessage(const std::string& message) {
    return json::parse(message);
}

json MessageHandler::handleMessage(const json& parsed_message, sqlite3* db, int client_socket) {
    std::string type = parsed_message["type"];
    json response_data;

    if (type == "SIGN_UP") {
        response_data = handleSignUp(parsed_message, db);
    } else if (type == "SIGN_IN") {
        response_data = handleSignIn(parsed_message, db, client_socket);
    } else if (type == "VIEW_ROOMS") {
        response_data = handleViewRooms(parsed_message, db);
    } else if (type == "CREATE_ROOM") {
        response_data = handleCreateRoom(parsed_message, db);
    } else if (type == "JOIN_ROOM") {
        response_data = handleJoinRoom(parsed_message, db);
    } else if (type == "LEAVE_ROOM") {
        response_data = handleLeaveRoom(parsed_message, db);
    // } else if (type == "READY") {
    //     response_data = handleReady(parsed_message, db);
    } else if (type == "START_GAME") {
        response_data = handleStartGame(parsed_message, db);
    } else if (type == "END_GAME") {
        response_data = handleEndGame(parsed_message, db);
    } else if (type == "VIEW_USERS") {
        response_data = handleViewUsers(parsed_message, db);
    } else if (type == "BAN_PLAYER") {
        response_data = handleBanPlayer(parsed_message, db);
    } else if (type == "SPECTATE") {
        response_data = handleSpectate(parsed_message, db);
    } else {
        return craftResponse("error", {{"message", "Unknown message type"}});
    }
    return craftResponse("success", response_data);
}

json MessageHandler::craftResponse(const std::string& status, const json& data) {
    return json{{"status", status}, {"data", data}};
}


json MessageHandler::handleSignUp(const json& parsed_message, sqlite3* db) {
    std::string username = parsed_message["username"];
    std::string password = parsed_message["password"];
    return userService.signUp(username, password);
}

json MessageHandler::handleSignIn(const json& parsed_message, sqlite3* db, int client_socket) {
    std::string username = parsed_message["username"];
    std::string password = parsed_message["password"];
    return userService.signIn(username, password, client_socket);
}

// cả admin và player đều đang có hàm này
json MessageHandler::handleViewRooms(const json& parsed_message, sqlite3* db) {
    playerService.viewRoomList();
}

json MessageHandler::handleCreateRoom(const json& parsed_message, sqlite3* db) {
    int player_id = parsed_message["player_id"];
    int max_players = parsed_message["max_players"];
    int max_spectators = parsed_message["max_spectators"];
    return playerService.createRoom(player_id, max_players, max_spectators);
}

json MessageHandler::handleJoinRoom(const json& parsed_message, sqlite3* db) {
    int player_id = parsed_message["player_id"];
    int room_id = parsed_message["room_id"];
    std::string participant_type_str = parsed_message["participant_type"];
    RoomParticipantStatus participant_type = stringToRoomParticipantStatus(participant_type_str);
    return playerService.joinRoom(player_id,room_id,participant_type);
}

json MessageHandler::handleLeaveRoom(const json& parsed_message, sqlite3* db) {
    int room_id = parsed_message["room_id"];
    std::string participant_type = parsed_message["participant_type"];
    int participant_id = parsed_message["participant_id"];
    bool is_ready = parsed_message["is_ready"];
    roomService.removeParticipant(room_id,participant_id);
}

// json MessageHandler::handleReady(const json& parsed_message, sqlite3* db) {
//     int room_id = parsed_message["room_id"];
//     std::string participant_type = parsed_message["participant_type"];
//     int participant_id = parsed_message["participant_id"];
//     bool is_ready = parsed_message["is_ready"];
//     RoomParticipant roomParticipant(room_id, RoomParticipantStatus::PLAYER, participant_id, is_ready);
//     return roomParticipant.isReady();
// }

json MessageHandler::handleStartGame(const json& parsed_message, sqlite3* db) {
    int player_id = parsed_message["player_id"];
    std::string initial_cube_state = parsed_message["initial_cube_state"];
    int room_id = parsed_message["room_id"];
    int max_players = parsed_message["max_players"];
    int max_spectators = parsed_message["max_spectators"];
    std::string status = parsed_message["status"];
    return roomService.startGameSession(player_id, room_id);
}

json MessageHandler::handleEndGame(const json& parsed_message, sqlite3* db) {
    int player_id = parsed_message["player_id"];
    int game_session_id = parsed_message["game_session_id"];
    int room_id = parsed_message["room_id"];
    int total_moves = parsed_message["total_moves"];
    int time_taken = parsed_message["time_taken"];
    
    return gameService.endGameSession(game_session_id, player_id);
}

json MessageHandler::handleViewUsers(const json& parsed_message, sqlite3* db) {
    int admin_id = parsed_message["admin_id"];
    vector<Player> players = playerRepo.getAllPlayers();
    //Craft json here
}

json MessageHandler::handleBanPlayer(const json& parsed_message, sqlite3* db) {
    int admin_id = parsed_message["admin_id"];
    int player_id = parsed_message["player_id"];
    return adminService.banPlayer(player_id,admin_id);
    // dang ra phai truyen them admin_id de dua vao field ban_by
}

json MessageHandler::handleSpectate(const json& parsed_message, sqlite3* db) {
    int admin_id = parsed_message["admin_id"];
    int room_id = parsed_message["room_id"];
    int participant_id = parsed_message["participant_id"];

    return adminService.spectate(room_id, participant_id);
}