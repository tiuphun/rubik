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
#include <string.h>

using namespace std;
using namespace std::chrono;

nlohmann::json Room::startGameSession(int player_id, const string initial_cube_state, int room_id) {
    // Check if game session is already started

    int player_game_session_id = server.player_game_session_id_counter;
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    GameSession new_game_session = GameSession(Server::game_session_id_counter, room_id, player_id, now_time_t ,0, 0, GameSessionStatus::IN_SESSION, initial_cube_state);
    nlohmann::json game_session_json = {
        {"id", new_game_session.id},
        {"room_id", new_game_session.room_id},
        {"player_id", new_game_session.player_id},
        {"start_time", new_game_session.start_time},
        {"end_time", new_game_session.end_time},
        {"status", new_game_session.status},
        {"initial_cube_state", new_game_session.initial_cube_state}
    };
    return MessageHandler::craftResponse("success", {{"game_session", game_session_json}});
}

std::string Room::initCubeState(int game_session_id) {
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