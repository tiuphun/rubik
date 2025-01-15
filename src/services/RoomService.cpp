#include "RoomService.h"
#include "../messages/MessageCrafter.h"
#include <chrono>
#include "GenCube.h"

using namespace std::chrono;
using namespace std;

nlohmann::json RoomService::startGameSession(int playerId, int roomId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageCrafter::craftResponse("error", {{"message", "Room not found"}});
    }

    /*
    if (!isPlayerInRoom(roomId, playerId)) {
        return MessageCrafter::craftResponse("error", {{"message", "Player not in room"}});
    }
    */
    

    string initialCubeState = initCubeState();
    time_t startTime = system_clock::to_time_t(system_clock::now());

    auto newSession = make_unique<GameSession>(
        entityManager.getNextGameSessionId(),
        roomId,
        playerId,
        startTime,
        0,
        0,
        IN_SESSION,
        initialCubeState
    );

    auto sessionJson = newSession->toJson();
    entityManager.addGameSession(std::move(newSession));

    return MessageCrafter::craftResponse("success", {
        {"message", sessionJson}
    });
}

nlohmann::json RoomService::removeParticipant(int roomId, int participantId) {
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageCrafter::craftResponse("error", {{"message", "Room not found"}});
    }

    auto participant = entityManager.getRoomParticipantById(participantId);
    if(!participant){
        return MessageCrafter::craftResponse("error", {{"message", "Participant not found"}});
    }

    if(participant->participant_type == RoomParticipantStatus::PLAYER){
        room->current_players--;
    }else{
        room->current_spectators--;
    }
    entityManager.removeRoomParticipant(participantId);
    return MessageCrafter::craftResponse("success", {{"message", "Participant removed"}});
}

std::string RoomService::initCubeState() {
    std::string output; // initial string for storing cube state
    
    Cube cube_state = Random_Cube(output);

    if (output.length() <= 0) {
        return MessageCrafter::craftResponse("error", {{"message", "Cannot init cube"}});
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
        return MessageCrafter::craftResponse("error", {{"message", "Room not found"}});
    }

    // Check capacity
    if (type == RoomParticipantStatus::PLAYER && room->current_players >= room->max_players) {
        return MessageCrafter::craftResponse("error", {{"message", "No player slots available"}});
    }
    if (type == RoomParticipantStatus::PLAYER_SPECTATOR && room->current_spectators >= room->max_spectators) {
        return MessageCrafter::craftResponse("error", {{"message", "No spectator slots available"}});
    }

    //Create & add participant
    auto participant = make_unique<RoomParticipant>(
        roomId,
        type,
        playerId,
        false  // not ready initially
    );

    entityManager.addRoomParticipant(std::move(participant));
    if (type == RoomParticipantStatus::PLAYER) {
        room->current_players++;
    } else {
        room->current_spectators++;
    }

    return MessageCrafter::craftResponse("success", {
        {"message", "Participant added successfully"},
        {"room_id", roomId}
    });
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
        return MessageCrafter::craftResponse("error", {{"message", "Room not found"}});
    }
    return MessageCrafter::craftResponse("success", {{"message", "Room to JSON Here"}});// change this.
}

nlohmann::json RoomService::toJson(int roomId) const {
    // Get room
    auto* room = entityManager.getRoomById(roomId);
    if (!room) {
        return MessageCrafter::craftResponse("error", {{"message", "Room not found"}});
    }

    // Get participants for this room
    nlohmann::json participantsJson = nlohmann::json::array();
    const auto& participants = entityManager.getRoomParticipants(roomId);
    for (const auto* participant : participants) {
        participantsJson.push_back({
            {"id", participant->id},
            {"player_id", participant->participant_id},
            {"type", participant->participant_type},
            {"is_ready", participant->is_ready}
        });
    }

    // Get game sessions for this room
    nlohmann::json gameSessionsJson = nlohmann::json::array();
    const auto& gameSessions = entityManager.getRoomGameSessions(roomId);
    for (const auto* session : gameSessions) {
        gameSessionsJson.push_back({
            {"id", session->id},
            {"player_id", session->player_id},
            {"start_time", session->start_time},
            {"cube_state", session->initial_cube_state},
            {"moves_count", session->total_moves}
        });
    }

    // Build complete room JSON
    nlohmann::json roomJson = {
        {"id", room->id},
        {"created_by", room->created_by},
        {"created_at", room->created_at},
        {"max_players", room->max_players},
        {"max_spectators", room->max_spectators},
        {"current_players", room->current_players},
        {"current_spectators", room->current_spectators},
        {"status", room->status},
        {"participants", participantsJson},
        {"game_sessions", gameSessionsJson}
    };

    return MessageCrafter::craftResponse("success", {{"room", roomJson}});
}