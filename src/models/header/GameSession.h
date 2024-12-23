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

    void createPlayerGameSessions(string initial_cube_state, int game_session_id, int playerIds[]){
        //Tao ra n PlayerGameSession = n player_id.
        //Tao ra 1 CubeState ban dau
    }

    void endGameSession(int player_id) {
        // if winner_id not null -> game end luon
        // other player an cuc :V
        // display result: Server select RoomPlayers & PlayerGameSession 
        // De lay ra id, username & completion_time
        // Send to client, client process.
    };

};

#endif