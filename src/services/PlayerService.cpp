// PlayerService.cpp
#include "PlayerService.h"
#include "../messages/MessageCrafter.h"
#include <chrono>
#include "../models/header/RoomParticipant.h"
#include "../models/header/Room.h"
#include "../constants/Const.h"

using namespace std::chrono;
using namespace std;

nlohmann::json PlayerService::createRoom(int playerId, int max_players, int max_spectators) {
    // Get player from entitymanager vector
    auto* player = entityManager.getPlayerById(playerId);
    if (!player) {
        return MessageCrafter::craftResponse("error", {{"message", "Player not found"}});
    }

    auto now = system_clock::now();
    auto nowTimeT = system_clock::to_time_t(now);

    // Create room and add to EntityManager
    auto room = make_unique<Room>(
        entityManager.getNextRoomId(),
        playerId,
        nowTimeT,
        max_players,
        max_spectators,
        RoomStatus::WAITING,
        1,
        0
    );

    int roomId = room->id;
    entityManager.addRoom(std::move(room));
    // New player is also the new RoomParticipant
    auto participant = make_unique<RoomParticipant>(
        roomId,
        RoomParticipantStatus::PLAYER,
        playerId,
        false
    );

    entityManager.addRoomParticipant(std::move(participant));
   

    return MessageCrafter::craftResponse("success", {
        {"message", "Room created successfully"},
        {"room_id", roomId}
    });
}

nlohmann::json PlayerService::joinRoom(int playerId, int roomId, RoomParticipantStatus participant_type) {
    // Get player and room from EntityManager
    auto* player = entityManager.getPlayerById(playerId);
    auto* room = entityManager.getRoomById(roomId);

    if (!player || !room) {
        return MessageCrafter::craftResponse("error", {
            {"message", "Player or room not found"}
        });
    }

    if (room->status != RoomStatus::WAITING) {
        return MessageCrafter::craftResponse("error", {
            {"message", "Room is not in WAITING state"}
        });
    }

    // Check capacity based on participant type
    if (participant_type == RoomParticipantStatus::PLAYER && 
        room->current_players >= room->max_players) {
        return MessageCrafter::craftResponse("error", {
            {"message", "Room is full"}
        });
    }

    if (participant_type == RoomParticipantStatus::PLAYER_SPECTATOR && 
        room->current_spectators >= room->max_spectators) {
        return MessageCrafter::craftResponse("error", {
            {"message", "Room spectator slots are full"}
        });
    }

    // Add participant to room
    auto participant = make_unique<RoomParticipant>(
        roomId,
        participant_type,
        playerId,
        false
    );

    entityManager.addRoomParticipant(std::move(participant));
    if (participant_type == RoomParticipantStatus::PLAYER) {
        room->current_players++;
    } else {
        room->current_spectators++;
    }

    return MessageCrafter::craftResponse("success", {
        {"message", "Joined room successfully"},
        {"room_id", roomId}
    });
}

nlohmann::json PlayerService::viewRoomList() {
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

void PlayerService::updatePlayerSocket(int playerId, int socketFd) {
    auto* player = entityManager.getPlayerById(playerId);
    if (player) {
        player->socket_fd = socketFd;
    }
}