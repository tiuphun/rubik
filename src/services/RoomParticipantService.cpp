#include "RoomParticipantService.h"
#include <iostream>

using namespace std;
nlohmann::json RoomParticipantService::leaveRoom(int participantId, int roomId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageCrafter::craftResponse("error", {{"message", "Room not found"}});
    }
    entityManager.removeRoomParticipant(participantId);

    cout << "Participant with id:" << participantId << "left the room with id:" << roomId << endl;
    return MessageCrafter::craftResponse("success", 
        {{"message", "Participant " + std::to_string(participantId) + " left the room"}});
}
