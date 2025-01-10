#include "AdminService.h"
#include "../database/queries/Query.h"
#include <iostream>

using namespace std;

nlohmann::json AdminService::banPlayer(int player_id, int admin_id) {
    const char* sql = Query::BAN_PLAYER;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(playerRepo.getDb(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(playerRepo.getDb()) << endl;
        return {{"status", "error"}, {"message", "Failed to prepare statement"}};
    }

    sqlite3_bind_int(stmt, 1, admin_id);
    sqlite3_bind_int(stmt, 2, player_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(playerRepo.getDb()) << endl;
        sqlite3_finalize(stmt);
        return {{"status", "error"}, {"message", "Failed to execute statement"}};
    }

    sqlite3_finalize(stmt);
    return {{"status", "success"}, {"message", "Player banned successfully"}};
}

nlohmann::json AdminService::viewPlayerList() {
    return playerRepo.getAllPlayers();
}

nlohmann::json Admin::banPlayer(int player_id, AdminService& adminService) {
    return adminService.banPlayer(player_id, this->id);
}

nlohmann::json Admin::viewPlayerList(AdminService& adminService) {
    return adminService.viewPlayerList();
}

nlohmann::json Admin::viewRoomList(Server& server) {
    vector<Room> rooms = server.getRooms();
    nlohmann::json rooms_json = nlohmann::json::array();
    for (const auto& room : rooms) {
        rooms_json.push_back(room.toJson());
    }
    return MessageHandler::craftResponse("success", {{"rooms", rooms_json}});
}

nlohmann::json Admin::spectate(int game_session_id, int room_id, Server& server) {
    // Implement spectate logic
    return {{"status", "success"}, {"message", "Spectating game session"}};
}

nlohmann::json Admin::leaveGame(Server& server) {
    // Implement leave game logic
    return {{"status", "success"}, {"message", "Admin left the game"}};
}