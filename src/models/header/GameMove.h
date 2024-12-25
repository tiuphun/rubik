#ifndef GAMEMOVE_H
#define GAMEMOVE_H

#include <string>
#include <ctime>
#include <string.h>

using namespace std;


class GameMove {
public:
    int id;
    int player_game_session_id;
    string move;
    time_t move_timestamp;
};

#endif