#include "RoomParticipantService.h"
#include <iostream>

using namespace std;
nlohmann::json RoomParticipantService::leaveRoom(int participantId, int roomId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageHandler::craftResponse("error", {{"message", "Room not found"}});
    }
    entityManager.removeRoomParticipant(participantId);

    cout << "Participant with id:" << participantId << "left the room with id:" << roomId << endl;
    return MessageHandler::craftResponse("success", 
        {{"message", "Participant " + std::to_string(participantId) + " left the room"}});
}
