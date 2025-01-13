#include "AdminService.h"
#include "../database/queries/Query.h"
#include <iostream>
#include "../messages/MessageHandler.h"

using namespace std;

nlohmann::json AdminService::viewPlayerList() {
    std::vector<Player> players = playerRepo.getAllPlayers();
    //Construct player json from database here.
    return MessageHandler::craftResponse({"success"}, {"message", "PLAYER LIST HERE"});
}

nlohmann::json AdminService::banPlayer(int player_id,int admin_id) {
    Player *player = entityManager.getPlayerById(player_id);
    player->status = PlayerStatus::BANNED;
    player->ban_by = admin_id;

    bool ban_ok = adminRepo.banPlayer(player_id, admin_id);
    if(ban_ok){
        return MessageHandler::craftResponse({"success"}, {"message", "Player banned successfully"});
    }else{
        return MessageHandler::craftResponse({"error"}, {"message", "Failed to update ban status to db"});
    }
}

nlohmann::json AdminService::viewRoomList() {
    const auto& rooms = entityManager.getAllRooms();
    nlohmann::json roomsJson = nlohmann::json::array();

    for (const auto& room : rooms) {
        if (room->status == RoomStatus::WAITING) {
            // Create room info JSON
            nlohmann::json roomInfo = {
                {"id", room->id},
                {"created_by", room->created_by},
                {"created_at", room->created_at},
                {"max_players", room->max_players},
                {"max_spectators", room->max_spectators},
                {"current_players", room->current_players},
                {"current_spectators", room->current_spectators},
                {"status", room->status}
            };
            
            roomsJson.push_back(roomInfo);
        }
    }

    return MessageHandler::craftResponse("success", {
        {"rooms", roomsJson}
    });
}

nlohmann::json AdminService::spectate(int game_session_id, int room_id) {
    GameSession *gs = entityManager.getGameSessionbyId(game_session_id);
    if(gs){
        //CRAFT GAME SESSION DETAILS HERE
        return MessageHandler::craftResponse("success","GAMESESSION");
    }else{
        return MessageHandler::craftResponse("error","Cannot spectate the game");
    }
} 

nlohmann::json AdminService::leaveGame(int admin_id) {
    //DISCONNECT Admin Socket HERE
    entityManager.removeAdmin(admin_id);
    cout << "Admin with id:" << admin_id << "left the game";
    return MessageHandler::craftResponse("success","Admin left the game");
}

bool AdminService::updateAdminSocket(int adminId, int socketFd) {
    //UPDATE Admin Socket Here and returns the isSuccessful boolean result
    bool isSuccessful = true;
    if(isSuccessful){
        cout << "Admin with id:" << adminId << "has been given a socket";
    }else{
        cout << "Failed to give socket to admin with id:" << adminId;
    }
    return true; //MODIFY IT TO CHECK IF THE SOCKET IS ASSIGNED SUCCESSFULLY
}