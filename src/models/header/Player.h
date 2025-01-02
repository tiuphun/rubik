#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <ctime>
#include "Const.h"
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"
#include "../server/Server.h"
using namespace std;

class Server;
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

    Server& server;
    Player(Server& srv) : server(srv) { socket_fd = -1; }

    Player(int id, const string& username, const string& password_hash, time_t join_date, int total_games, int wins, float best_time, float avg_time, PlayerStatus status, time_t ban_date, int ban_by, Server& server)
        : id(id), username(username), password_hash(password_hash), join_date(join_date), total_games(total_games), wins(wins), best_time(best_time), avg_time(avg_time), status(status), ban_date(ban_date), ban_by(ban_by), socket_fd(-1), server(server) {}

    nlohmann::json createRoom(int max_players, int max_spectators, Server& server);
    nlohmann::json joinRoom(int room_id,  RoomParticipantStatus participant_type, Server& server);
    nlohmann::json viewRoomList(Server& server);

    sqlite3* db;
};

#endif