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

    Room(int id, int created_by, time_t created_at, int max_players, int max_spectators, RoomStatus status)
        : game_session(), id(id), created_by(created_by), created_at(created_at), max_players(max_players), max_spectators(max_spectators), status(status) {}

    json startGameSession(int game_session_id, int player_id, string initial_cube_state);
    json canStartGame();
    json initCubeState();

    int getId() const { return id; }
    int getCreatedBy() const { return created_by; }
    time_t getCreatedAt() const { return created_at; }
    int getMaxPlayers() const { return max_players; }
    int getMaxSpectators() const { return max_spectators; }
    RoomStatus getStatus() const { return status; }
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