#ifndef PLAYERGAMESESSION_H
#define PLAYERGAMESESSION_H

#include <stdio.h>
#include <string.h>
#include <string>
#include "Const.h"
#include <ctime>

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

    bool validateCube(string cube) {
        // re-implement algo check cube cua Thinh
        // Dua vao cube state gan nhat tron cube_state_id, dua vao 5 move sequence de xem co ra 
        //cai thang client gui ko. Neu co, thi  update bang CubeState(goi updateCubeFromClient), update int cube_state_id & validated = true,; 
        // Neu khong, notify thang player (nhung van cho choi tiep).
        //Neu cube cuoi cung duoc validate la cube 6 mat cung mau => gui request yeu cau client stop timer & update completion_time
        
    };

    void updateMoveFromClient(string message) {
        // parse message -> move
        // check move xem co valid k 
        // update vao db
    }

    void updateCubeFromClient(string message) {
        // parse message -> move
        // update vao db
    }
  
};

#endif