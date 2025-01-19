#ifndef ROOM_PARTICIPANT_SERVICE_H
#define ROOM_PARTICIPANT_SERVICE_H

#include <nlohmann/json.hpp>
#include "../models/header/EntityManager.h"
#include "../models/header/Room.h"
#include "../models/header/RoomParticipant.h"
#include "../messages/MessageCrafter.h"

class RoomParticipantService {
public:
    RoomParticipantService(EntityManager& entityManager)
        : entityManager(entityManager) {}

    nlohmann::json leaveRoom(int participantId, int roomId);
private:
    EntityManager& entityManager;
};

#endif