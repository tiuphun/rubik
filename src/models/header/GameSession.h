#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <ctime>
#include "Const.h"

using namespace std;

class GameSession {
public:
    int id;
    int room_id;
    time_t start_time;
    time_t end_time;
    int total_moves;
    GameSessionStatus status;
    string initial_cube_state;
    int winner_id;
};

#endif