#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "PlayerGameSession.h"

// return bool
void validateCube(string cube) {
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