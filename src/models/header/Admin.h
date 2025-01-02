#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class Admin {
public:
    int id;
    string username;
    string password_hash;
    time_t created_at;
    time_t last_login; 
    int socket_fd;
    sqlite3* db;

    Admin(int id, const string& username, const string& password_hash, time_t created_at, time_t last_login)
        : id(id), username(username), password_hash(password_hash), created_at(created_at), last_login(last_login), socket_fd(-1) {}


    nlohmann::json banPlayer(int player_id);
    nlohmann::json viewPlayerList();
    nlohmann::json viewRoomList();
    nlohmann::json spectate(int game_session_id, int room_id);
    nlohmann::json leaveGame(int room_id);


   
};

#endif