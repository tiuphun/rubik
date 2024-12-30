// include/Room.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "Const.h"
#include "RoomParticipant.h"
#include "GameSession.h"
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
    
    Room(sqlite3* db) : db(db) {}

    json startGameSession(int game_session_id, int player_id, string initial_cube_state);
    json canStartGame();
    json initCubeState();
    
private:
    sqlite3* db;
};

#endif
