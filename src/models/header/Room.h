#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <string>
#include <ctime>
#include "RoomParticipant.h"
#include "GameSession.h"
#include "Const.h"
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class Room {
public:
    int id;
    int created_by;
    time_t created_at;
    int max_players;
    int max_spectators;
    RoomStatus status;
    int current_players = 0;
    int current_spectators = 0;


    Room(int id, int created_by, time_t created_at, 
         int max_players, int max_spectators, 
         RoomStatus status,
         int current_players = 0, 
         int current_spectators = 0)
        : id(id)
        , created_by(created_by)
        , created_at(created_at)
        , max_players(max_players)
        , max_spectators(max_spectators)
        , status(status)
        , current_players(current_players)
        , current_spectators(current_spectators) {}

};

#endif