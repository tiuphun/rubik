#include "Admin.h"
#include "../../services/AdminService.h"
#include "../../server/Server.h"
#include "../../messages/MessageHandler.h"

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