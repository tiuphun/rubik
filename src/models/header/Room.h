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
    GameSession game_session;
    int id;
    int created_by;
    time_t created_at;
    int max_players;
    int max_spectators;
    RoomStatus status;
    int current_players = 0;
    int current_spectators = 0;

    Server& server;

    Room(int id, int created_by, time_t created_at, int max_players, int max_spectators, RoomStatus status, Server& server, int current_players = 0, int current_spectators = 0)
        : game_session(), id(id), created_by(created_by), created_at(created_at), max_players(max_players), max_spectators(max_spectators), status(status), current_players(current_players), current_spectators(current_spectators), server(server) {}

    nlohmann::json startGameSession(int game_session_id, int player_id, string initial_cube_state);
    nlohmann::json canStartGame();
    nlohmann::json initCubeState();
    nlohmann::json addRoomParticipant(RoomParticipant room_participant);
    nlohmann::json removeRoomParticipant(int participant_id);
    RoomParticipant findParticipantById(int participant_id);
    string getStatusString() const {
        switch (status) {
            case RoomStatus::WAITING: return "WAITING";
            case RoomStatus::READY: return "READY";
            case RoomStatus::IN_PROGRESS: return "IN_PROGRESS";
            case RoomStatus::ENDED: return "ENDED";
            default: return "UNKNOWN";
        }
    }

    sqlite3* db;
};

#endif