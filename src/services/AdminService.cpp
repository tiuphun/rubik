#include "AdminService.h"
#include "../database/queries/Query.h"
#include <iostream>
#include "../messages/MessageCrafter.h"

using namespace std;

nlohmann::json AdminService::viewPlayerList() {
    std::vector<Player> players = playerRepo.getAllPlayers();
    //Construct player json from database here.
    return MessageCrafter::craftResponse({"success"}, {"message", "PLAYER LIST HERE"});
}

nlohmann::json AdminService::banPlayer(int player_id,int admin_id) {
    Player *player = entityManager.getPlayerById(player_id);
    player->status = PlayerStatus::BANNED;
    player->ban_by = admin_id;

    bool ban_ok = adminRepo.banPlayer(player_id, admin_id);
    if(ban_ok){
        return MessageCrafter::craftResponse({"success"}, {"message", "Player banned successfully"});
    }else{
        return MessageCrafter::craftResponse({"error"}, {"message", "Failed to update ban status to db"});
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

    return MessageCrafter::craftResponse("success", {
        {"rooms", roomsJson}
    });
}

nlohmann::json AdminService::spectate(int game_session_id, int room_id, int adminSocketFd) {
    GameSession *gs = entityManager.getGameSessionbyId(game_session_id);
    Room *room = entityManager.getRoomById(room_id);
    if(gs && room){
        //CRAFT GAME SESSION DETAILS HERE
        auto newSpectator = make_unique<RoomParticipant>(
            room_id,
            RoomParticipantStatus::ADMIN_SPECTATOR,
            gs->player_id,
            false
        );

        entityManager.addRoomParticipant(std::move(newSpectator));
        room->current_spectators++;

        // Start periodic updates
        return gameService.startPeriodicUpdates(game_session_id, adminSocketFd, 5); // Update every 5 seconds
        
        // return MessageCrafter::craftResponse("success","Admin is now spectating the game");
    }else{
        return MessageCrafter::craftResponse("error","Cannot spectate the game");
    }
} 

nlohmann::json AdminService::leaveGame(int admin_id) {
    //DISCONNECT Admin Socket HERE
    entityManager.removeAdmin(admin_id);
    cout << "Admin with id:" << admin_id << "left the game";

    // Stop periodic updates
    gameService.stopPeriodicUpdates();
    
    return MessageCrafter::craftResponse("success","Admin left the game");
}

bool AdminService::updateAdminSocket(int adminId, int socketFd) {
    auto* admin = entityManager.getAdminById(adminId);
    if (admin) {
        admin->socket_fd = socketFd;
    }

    if(admin->socket_fd == -1){
        cout << "Failed to give socket to admin with id:" << adminId;
        return false;
    }else{
        cout << "Admin with id:" << adminId << "has been given a socket";
        return true;
    }
}