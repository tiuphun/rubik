#include "RoomService.h"
#include "../messages/MessageHandler.h"
#include <chrono>
#include "GenCube.h"

using namespace std::chrono;
using namespace std;

nlohmann::json RoomService::startGameSession(int playerId, int roomId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageHandler::craftResponse("error", {{"message", "Room not found"}});
    }

    if (!isPlayerInRoom(roomId, playerId)) {
        return MessageHandler::craftResponse("error", {{"message", "Player not in room"}});
    }

    string initialCubeState = initCubeState();
    time_t startTime = system_clock::to_time_t(system_clock::now());

    GameSession newSession(
        entityManager.getNextGameSessionId(),
        roomId,
        playerId,
        startTime,
        0,
        0,
        IN_SESSION,
        initialCubeState
    );

    room->game_sessions.push_back(newSession);

    return MessageHandler::craftResponse("success", {
        {"game_session", newSession.toJson()}
    });
}

std::string RoomService::initCubeState() {
    std::string output; // initial string for storing cube state
    
    Cube cube_state = Random_Cube(output);

    if (output.length() <= 0) {
        return MessageHandler::craftResponse("error", {{"message", "Cannot init cube"}});
    }

    std::string cube_state_string = "";

    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 8; j++){
            cube_state_string = cube_state_string + cube_state.color[i][j] + " ";
        }
    }
    cube_state_string[cube_state_string.size() -1] = '\0';
    return cube_state_string;
}

nlohmann::json RoomService::addParticipant(int roomId, int playerId, RoomParticipantStatus type) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageHandler::craftResponse("error", {{"message", "Room not found"}});
    }

    if (isRoomFull(room, type)) {
        return MessageHandler::craftResponse("error", {{"message", "Room is full"}});
    }

    RoomParticipant participant(roomId, type, playerId, false);
    updateRoomCounters(room, type, true);
    room->participants.push_back(participant);

    return MessageHandler::craftResponse("success", {{"message", "Participant added successfully"}});
}

void RoomService::updateRoomCounters(Room* room, RoomParticipantStatus type, bool isAdding) {
    if (type == PLAYER) {
        room->current_players += isAdding ? 1 : -1;
    } else {
        room->current_spectators += isAdding ? 1 : -1;
    }
}

bool RoomService::isRoomFull(const Room* room, RoomParticipantStatus type) const {
    if (type == PLAYER) {
        return room->current_players >= room->max_players;
    }
    return room->current_spectators >= room->max_spectators;
}

nlohmann::json RoomService::getRoomDetails(int roomId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageHandler::craftResponse("error", {{"message", "Room not found"}});
    }
    return MessageHandler::craftResponse("success", room->toJson());
}