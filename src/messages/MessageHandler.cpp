#include "MessageHandler.h"

json MessageHandler::parseMessage(const std::string& message) {
    return json::parse(message);
}

json MessageHandler::handleMessage(const json& parsed_message, sqlite3* db) {
    std::string type = parsed_message["type"];
    json response_data;

    // if (type == "BAN_PLAYER") {
    //     response_data = ActionHandlers::handle_ban_player(parsed_message, db);
    // } else if (type == "VIEW_PLAYER_LIST") {
    //     response_data = ActionHandlers::handle_view_player_list(db);
    // } else if (type == "VIEW_ROOM_LIST") {
    //     response_data = ActionHandlers::handle_view_room_list(db);
    // } else if (type == "SPECTATE") {
    //     response_data = ActionHandlers::handle_spectate(parsed_message, db);
    // } else if (type == "LEAVE_GAME") {
    //     response_data = ActionHandlers::handle_leave_game(parsed_message, db);
    // } else {
    //     return craft_response("error", {{"message", "Unknown message type"}});
    // }

    // return craft_response("success", response_data);
}

json MessageHandler::craftResponse(const std::string& status, const json& data) {
    return json{{"status", status}, {"data", data}};
}