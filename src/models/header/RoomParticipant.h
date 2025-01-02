#ifndef ROOMPARTICIPANT_H
#define ROOMPARTICIPANT_H

#include <string.h>
#include <stdio.h>
#include <ctime>
#include <sqlite3.h>
#include "Const.h"
#include <string>
#include <chrono>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class Server;
class RoomParticipant {
public:
    int id;
    int room_id;
    RoomParticipantStatus participant_type;
    int participant_id;
    bool is_ready;
    time_t joined_at;
    RoomParticipant(int room_id, RoomParticipantStatus participant_type, int participant_id, bool is_ready)
        : room_id(room_id), participant_type(participant_type), participant_id(participant_id), is_ready(is_ready), joined_at(getCurrentTime()) {}

    nlohmann::json leaveRoom(Server &server);
    nlohmann::json isReady();

    static time_t getCurrentTime() {
        using namespace std::chrono;
        return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    }
    
private:
    sqlite3* db;

    
};

#endif