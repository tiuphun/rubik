#include "RoomParticipantService.h"

nlohmann::json RoomParticipantService::leaveRoom(int participantId, int roomId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageHandler::craftResponse("error", {{"message", "Room not found"}});
    }

    if (!isValidParticipant(participantId, roomId)) {
        return MessageHandler::craftResponse("error", {{"message", "Participant not found"}});
    }

    room->removeRoomParticipant(participantId);
    return MessageHandler::craftResponse("success", 
        {{"message", "Participant " + std::to_string(participantId) + " left the room"}});
}
