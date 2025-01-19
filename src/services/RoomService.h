#ifndef ROOM_SERVICE_H
#define ROOM_SERVICE_H

#include "../models/header/EntityManager.h"
#include "../models/header/Room.h"
#include "../models/header/GameSession.h"
#include <nlohmann/json.hpp>

class RoomService {
public:
    RoomService(EntityManager& entityManager)
        : entityManager(entityManager) {}

    // Core room operations
    nlohmann::json startGameSession(int playerId, int roomId);
    nlohmann::json addParticipant(int roomId, int playerId, RoomParticipantStatus type);
    nlohmann::json removeParticipant(int roomId, int participantId);
    nlohmann::json getRoomDetails(int roomId);
    nlohmann::json toJson(int roomId) const;
    bool hasParticipant(int roomId, int participant_id);
    
    // Room state checks
    bool isRoomFull(const Room* room, RoomParticipantStatus type) const;
    //bool isPlayerInRoom(int roomId, int playerId) const;
    bool canStartGame(const Room* room, int playerId) const;

private:
    EntityManager& entityManager;
    std::string initCubeState();
    void updateRoomCounters(Room* room, RoomParticipantStatus type, bool isAdding);
};
#endif