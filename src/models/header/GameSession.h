#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <ctime>
#include "Const.h"
#include "PlayerGameSession.h"
#include <vector>
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

    vector <PlayerGameSession> player_game_sessions;
    GameSession(sqlite3* db) : db(db) {}

    json createPlayerGameSessions(const vector<int>& playerIds, int cube_state_id);
    json endGameSession(int player_id);

private:
    sqlite3* db;

};

#endif