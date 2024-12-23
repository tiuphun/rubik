// include/Room.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "Const.h"

using namespace std;

class Room {
public:
    int id;
    int created_by;
    time_t created_at;
    int max_players;
    int max_spectators;
    RoomStatus status;
    

    void startGameSession(int game_session_id, int player_id, string initial_cube_state);
    //Nhung gi minh can lam trong 3s countdown.
    
    bool canStartGame(int room_id) {
        // check dieu kien (1) du player, (2) everi player readyyy
        // gui response to client
        // bat dau startGameSession(game_session_id, player_id, initCubeState())
    };

    string initCubeState(){
        //return cube state ban dau
    }
    
};

#endif
