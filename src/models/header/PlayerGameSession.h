#ifndef PLAYERGAMESESSION_H
#define PLAYERGAMESESSION_H

#include <stdio.h>
#include <string.h>
#include <string>
#include "Const.h"
#include <ctime>
#include <sqlite3.h>

using namespace std;

class PlayerGameSession {
public:
    int id;
    int game_session_id;
    int player_id;
    time_t time_taken;
    int moves_count;
    string move_sequence;
    PlayerGameSessionStatus status;
    time_t completion_time;
    int cube_state_id;

    PlayerGameSession(sqlite3* db) : db(db) {}

private:
    void validateCube(string cube);
    void updateMoveFromClient(string message);
    void updateCubeFromClient(string message);
  
    sqlite3* db;
};

#endif