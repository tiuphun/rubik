#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <vector>
#include <string>
#include <ctime>
#include "PlayerGameSession.h"
#include "Const.h"
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"

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
    sqlite3* db;

    vector<PlayerGameSession> player_game_sessions;

    // Default constructor
    GameSession() : id(0), room_id(0), start_time(0), end_time(0), total_moves(0), status(GameSessionStatus::IN_SESSION), initial_cube_state(""), winner_id(0), db(nullptr) {}

    GameSession(int id, int room_id, time_t start_time, time_t end_time, int total_moves, GameSessionStatus status, const string& initial_cube_state, int winner_id, sqlite3* db)
        : id(id), room_id(room_id), start_time(start_time), end_time(end_time), total_moves(total_moves), status(status), initial_cube_state(initial_cube_state), winner_id(winner_id), db(db) {}

    nlohmann::json createPlayerGameSessions(const vector<int>& playerIds, int cube_state_id);
    nlohmann::json endGameSession(int player_id);
    
};

#endif