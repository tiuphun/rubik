#ifndef CUBESTATE_H
#define CUBESTATE_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <ctime>
#include "Const.h"

using namespace std;

class CubeState{
public: 
    int id;
    int player_game_session_id;
    string cube;
    time_t validation_timestamp;
    ValidationStatus validation_result;
};

#endif