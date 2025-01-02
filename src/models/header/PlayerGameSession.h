#ifndef PLAYERGAMESESSION_H
#define PLAYERGAMESESSION_H

#include <stdio.h>
#include <string.h>
#include <string>
#include "Const.h"
#include <ctime>
#include <sqlite3.h>
#include "CubeState.h"
#include <vector>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class PlayerGameSession {
public:
    vector<CubeState> cube_states;

    int id;
    int game_session_id;
    int player_id;
    time_t time_taken;
    int moves_count;
    string move_sequence;
    string status;
    time_t completion_time;
    int cube_state_id;

    PlayerGameSession(sqlite3* db) : db(db) {}

    nlohmann::json updateCubeFromClient(string message);
  
private:
    sqlite3* db;
};

#endif