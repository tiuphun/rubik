#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Room.h"
#include "Const.h"
#include "../../database/queries/Query.h"
#include "../../messages/MessageHandler.h"
#include <chrono>
#include "../../services/GenCube.h"

using namespace std;
using namespace std::chrono;

nlohmann::json Room::startGameSession(int game_session_id, int player_id, const string initial_cube_state) {
    // Check if game session is already started
    if (this->game_session.id != -1) {
        return MessageHandler::craftResponse("error", {{"message", "Game session already started"}});
    }

    int new_game_session_id = server.game_session_id_counter++;
    // Start game session
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    this->game_session = GameSession(new_game_session_id,this->id, now_time_t,0,0 , GameSessionStatus::IN_SESSION, initial_cube_state, 0);
    return MessageHandler::craftResponse("success", {{"message", "Game session with id: " + to_string(new_game_session_id) + "of Room id:" + to_string(this->id)+ " started successfully"}});
}

nlohmann::json Room::canStartGame() {
    bool canStart = true;
    for (const auto& participant : participants) {
        if (participant.participant_type == RoomParticipantStatus::PLAYER && !participant.is_ready) {
            bool canStart = false;
            break;
        }
    }
    if (canStart)
    {
        return MessageHandler::craftResponse("success", {{"message", "Game can start"}});
    } else {
        return MessageHandler::craftResponse("error", {{"message", "Game cannot start"}});
    }
}

nlohmann::json Room::initCubeState(int game_session_id) {
    std::string output; // initial string for storing cube state
    
    CubeState cube_state;
    cube_state.id = 1; 
    cube_state.player_game_session_id = game_session_id; 
    cube_state.cube = Random_Cube(output); 
    cube_state.validation_timestamp = std::time(nullptr); 
    cube_state.validation_result = ValidationStatus::VALID; 

    if (cube_state.cube == NULL) {
        return MessageHandler::craftResponse("error", {"message", "Cannot init cube"})
    }
    nlohmann::json initial_cube_state = {
        {"id", cube_state.id},
        {"player_game_session_id", cube_state.player_game_session_id},
        {"cube", cube_state.cube},
        {"validation_timestamp", cube_state.validation_timestamp},
        {"validation_result", cube_state.validation_result == ValidationStatus::VALID ? "VALID" : "INVALID"}
    };
    return MessageHandler::craftResponse("success", {{"message", "Cube state initialized successfully"}, {"initial_cube_state", cube_state_json}});

}

RoomParticipant Room::findParticipantById(int participant_id) {
    for (RoomParticipant& participant : this->participants) {
        if (participant_id == participant.id) {
            return participant;
        }
    }
    throw runtime_error("Participant not found and thus cannot be removed from room participant list");
}

nlohmann::json Room::removeRoomParticipant(int participant_id) {
    // Remove participant from room
    auto it = std::remove_if(this->participants.begin(), this->participants.end(), [participant_id](const RoomParticipant& participant) {
        return participant.participant_id == participant_id;
    });

    if (it != this->participants.end()) {
        this->participants.erase(it, this->participants.end());
        return MessageHandler::craftResponse("success", {{"message", "Participant removed from RoomParticipant successfully"}});
    } else {
        return MessageHandler::craftResponse("error", {{"message", "Participant not found"}});
    }
}