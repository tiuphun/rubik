#include "MessageHandler.h"
#include "../services/UserService.h"
#include "../models/header/Player.h"

json MessageHandler::parseMessage(const std::string& message) {
    return json::parse(message);
}

json MessageHandler::handleMessage(const json& parsed_message, sqlite3* db) {
    std::string type = parsed_message["type"];
    json response_data;

    if (type == "SIGN_UP") {
        response_data = handleSignUp(parsed_message, db);
    } else if (type == "SIGN_IN") {
        response_data = handleSignIn(parsed_message, db);
    } else if (type == "CREATE_ROOM") {
        response_data = handleCreateRoom(parsed_message, db);
    } else if (type == "JOIN_ROOM") {
        response_data = handleJoinRoom(parsed_message, db);
    } else {
        return craftResponse("error", {{"message", "Unknown message type"}});
    }

    return craftResponse("success", response_data);
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