#include "MessageHandler.h"
#include "../services/UserService.h"
#include "../models/header/Player.h"
#include "../models/header/Admin.h"
#include "../models/header/Room.h"
#include "../models/header/RoomParticipant.h"
#include "../models/header/PlayerGameSession.h"
#include "../models/header/GameSession.h"
#include <unordered_map>
#include <functional>

json MessageHandler::parseMessage(const std::string& message) {
    return json::parse(message);
}

json MessageHandler::handleMessage(const json& parsed_message, sqlite3* db) {
    std::string type = parsed_message["type"];
    json response_data;

    static const std::unordered_map<std::string, HandlerFunction> handlers = {
        {"SIGN_UP", [this](const json& msg, sqlite3* db) { return handleSignUp(msg["data"], db); }},
        {"SIGN_IN", [this](const json& msg, sqlite3* db) { return handleSignIn(msg["data"], db); }},
        {"VIEW_ROOMS", [this](const json& msg, sqlite3* db) { return handleViewRooms(msg["data"], db); }},
        {"CREATE_ROOM", [this](const json& msg, sqlite3* db) { return handleCreateRoom(msg["data"], db); }},
        {"JOIN_ROOM", [this](const json& msg, sqlite3* db) { return handleJoinRoom(msg["data"], db); }},
        {"LEAVE_ROOM", [this](const json& msg, sqlite3* db) { return handleLeaveRoom(msg["data"], db); }},
        {"READY", [this](const json& msg, sqlite3* db) { return handleReady(msg["data"], db); }},
        {"START_GAME", [this](const json& msg, sqlite3* db) { return handleStartGame(msg["data"], db); }},
        {"CUBE_UPDATE", [this](const json& msg, sqlite3* db) { return handleCube(msg["data"], db); }},
        {"END_GAME", [this](const json& msg, sqlite3* db) { return handleEndGame(msg["data"], db); }},
        {"VIEW_USERS", [this](const json& msg, sqlite3* db) { return handleViewUsers(msg["data"], db); }},
        {"BAN_PLAYER", [this](const json& msg, sqlite3* db) { return handleBanPlayer(msg["data"], db); }},
        {"SPECTATE", [this](const json& msg, sqlite3* db) { return handleSpectate(msg["data"], db); }},
        {"READY", [this](const json& msg, sqlite3* db) { return handleReady(msg["data"], db); }}
    };

    auto it = handlers.find(type);
    if (it != handlers.end()) {
        response_data = it->second(parsed_message, db);
        return craftResponse("success", response_data);
    } else {
        return craftResponse("error", {{"message", "Unknown message type"}});
    }
}

json MessageHandler::craftResponse(const std::string& status, const json& data) {
    return json{{"status", status}, {"data", data}};
}


json MessageHandler::handleSignUp(const json& parsed_message, sqlite3* db) {
    UserService userService(db);
    std::string username = parsed_message["username"];
    std::string password = parsed_message["password"];
    return userService.signUp(username, password);
}

json MessageHandler::handleSignIn(const json& parsed_message, sqlite3* db) {
    UserService userService(db);
    std::string username = parsed_message["username"];
    std::string password = parsed_message["password"];
    return userService.signIn(username, password);
}

json MessageHandler::handleCreateRoom(const json& parsed_message, sqlite3* db) {
    Player player(db);
    int max_players = parsed_message["max_players"];
    int max_spectators = parsed_message["max_spectators"];
    return player.createRoom(max_players, max_spectators);
}

json MessageHandler::handleJoinRoom(const json& parsed_message, sqlite3* db) {
    Player player(db);
    int room_id = parsed_message["room_id"];
    std::string participant_type = parsed_message["participant_type"];
    return player.joinRoom(room_id, participant_type.c_str());
}

json MessageHandler::handleReady(const json& parsed_message, sqlite3* db) {
    RoomParticipant roomParticipant(db);
    int room_id = parsed_message["room_id"];
    return roomParticipant.isReady(room_id);
}

json MessageHandler::handleLeaveRoom(const json& parsed_message, sqlite3* db) {
    RoomParticipant roomParticipant(db);
    return roomParticipant.leaveRoom();
}

json MessageHandler::handleViewUsers(const json& parsed_message, sqlite3* db) {
    Admin admin(db);
    return admin.viewPlayerList();
}

json MessageHandler::handleBanPlayer(const json& parsed_message, sqlite3* db) {
    Admin admin(db);
    int player_id = parsed_message["player_id"];
    return admin.banPlayer(player_id);
}

// cả admin và player đều đang có hàm này
json MessageHandler::handleViewRooms(const json& parsed_message, sqlite3* db) {
    Player player(db);
    return player.viewRooms();
}

json MessageHandler::handleStartGame(const json& parsed_message, sqlite3* db) {
    Player player(db);
    return player.startGame();
    //Missing gen cube function
}

json MessageHandler::handleEndGame(const json& parsed_message, sqlite3* db) {
    GameSession gameSession(db);
    int player_id = parsed_message["player_id"];
    return gameSession.endGameSession(player_id);
}

json MessageHandler::handleSpectate(const json& parsed_message, sqlite3* db) {
    Admin admin(db);
    int game_session_id = parsed_message["game_session_id"];
    int room_id = parsed_message["room_id"];
    return admin.spectate(game_session_id, room_id);
}