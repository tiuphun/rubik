#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <sqlite3.h>

using namespace std;

class Admin {
public:
    int id;
    string username;
    string password_hash;
    time_t created_at;
    time_t last_login; 

    Admin(sqlite3* db) : db(db) {}

private:
    void banPlayer(int player_id);
    void viewPlayerList();
    void viewRoomList();
    void spectate(int game_session_id, int room_id);
    void leaveGame(int room_id);

    sqlite3* db;
};

#endif