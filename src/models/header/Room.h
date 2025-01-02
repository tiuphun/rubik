#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <string>
#include <ctime>
#include "RoomParticipant.h"
#include "GameSession.h"
#include "Const.h"
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"
#include "../server/Server.h"

using namespace std;

class Room {
public:
    vector<RoomParticipant> participants;
    vector<GameSession> game_sessions;
    int id;
    int created_by;
    time_t created_at;
    int max_players;
    int max_spectators;
    RoomStatus status;
    int current_players = 0;
    int current_spectators = 0;

    Server& server;

    Room(int id, int created_by, time_t created_at, int max_players, int max_spectators, RoomStatus status, Server& server, vector<GameSession> game_session, int current_players = 0, int current_spectators = 0)
        :  id(id), created_by(created_by), created_at(created_at), max_players(max_players), max_spectators(max_spectators), status(status), current_players(current_players), current_spectators(current_spectators), server(server), game_sessions(game_session) {}

    nlohmann::json startGameSession(int player_id, string initial_cube_state, int room_id);
    string initCubeState(int game_session_id);
    nlohmann::json addRoomParticipant(RoomParticipant room_participant);
    nlohmann::json removeRoomParticipant(int participant_id);
    RoomParticipant findParticipantById(int participant_id);
    nlohmann::json toJson() const;
    sqlite3* db;
};

#endif