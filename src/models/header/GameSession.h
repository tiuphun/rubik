#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <vector>
#include <string>
#include <ctime>
#include "Const.h"
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class GameSession {
public:
    int id;
    int room_id;
    int player_id;
    time_t start_time;
    time_t end_time;
    int total_moves;
    GameSessionStatus status;
    string initial_cube_state;

    GameSession(int id, int room_id, int player_id, time_t start_time, time_t end_time, int total_moves, GameSessionStatus status, const string& initial_cube_state)
        : id(id), room_id(room_id), start_time(start_time), end_time(end_time), total_moves(total_moves), status(status), initial_cube_state(initial_cube_state) {}

    nlohmann::json endGameSession(int player_id);
    
};

#endif