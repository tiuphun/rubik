#ifndef ROOMPARTICIPANT_H
#define ROOMPARTICIPANT_H

#include <string.h>
#include <stdio.h>
#include <ctime>
#include <sqlite3.h>
#include "Const.h"
#include <string>
using namespace std;

class RoomParticipant {
public:
    int id;
    int room_id;
    //Remove enum usage
    string participant_type;
    int participant_id;
    bool is_ready;
    time_t joined_at;
    RoomParticipant(sqlite3* db) : db(db) {}


    void leaveRoom();
    void isReady(int room_id);
private:
    sqlite3* db;
};

#endif