// include/Room.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "Const.h"

using namespace std;

class Room {
public:
    int id;
    int created_by;
    time_t created_at;
    int max_players;
    int max_spectators;
    RoomStatus status;
    
    Room(sqlite3* db) : db(db) {}
    
private:
    void startGameSession(int game_session_id, int player_id, string initial_cube_state);
    bool canStartGame();
    string initCubeState();
    
    sqlite3* db;
};

#endif
