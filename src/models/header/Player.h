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

    // Constructor with all member variables

    Player(): id(0)
        , username("")
        , password_hash("")
        , join_date(std::time(nullptr))
        , total_games(0)
        , wins(0)
        , best_time(0.0f)
        , avg_time(0.0f)
        , status(PlayerStatus::INACTIVE)
        , ban_date(0)
        , ban_by(0)
        , socket_fd(-1) {}
    Player(int id, const string& username, const string& password_hash, time_t join_date, int total_games, int wins, float best_time, float avg_time, PlayerStatus status, time_t ban_date, int ban_by, int socket_fd)
        : id(id), username(username), password_hash(password_hash), join_date(join_date), total_games(total_games), wins(wins), best_time(best_time), avg_time(avg_time), status(status), ban_date(ban_date), ban_by(ban_by), socket_fd(socket_fd) {}
};

#endif // PLAYER_H