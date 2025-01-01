#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <ctime>
#include "Const.h"
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

    Player(int id, const string& username, const string& password_hash, time_t join_date, int total_games, int wins, float best_time, float avg_time, PlayerStatus status, time_t ban_date, int ban_by)
        : id(id), username(username), password_hash(password_hash), join_date(join_date), total_games(total_games), wins(wins), best_time(best_time), avg_time(avg_time), status(status), ban_date(ban_date), ban_by(ban_by), socket_fd(-1) {}

    json createRoom(int max_players, int max_spectators);
    json joinRoom(int room_id, const char* participant_type);
    json viewRoomList();

    sqlite3* db;
};

#endif