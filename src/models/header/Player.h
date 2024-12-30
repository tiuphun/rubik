// include/Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <string.h>
#include "Const.h"
#include <ctime>
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class Player {
public:
    int id;
    string username;
    string password_hash;
    time_t join_date;
    int total_games;
    int wins;
    float best_time;
    float avg_time;
    PlayerStatus status;
    time_t ban_date;
    int ban_by;
    int socket_fd;
    
    Player(sqlite3* db) : db(db) {}

    json createRoom(int max_players, int max_spectators);
    json joinRoom(int room_id, const char* participant_type);
    json viewRoomList();

    sqlite3* db;
};

#endif
