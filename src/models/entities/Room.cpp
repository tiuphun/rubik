#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Room.h"
#include "Const.h"
#include "../../database/queries/Query.h"

using namespace std;

json Room::startGameSession(int game_session_id, int player_id, string initial_cube_state) {
    // bat dau game session
    // tao game session
    // tao cube
    // tao game session player
    // tao game session cube
    // tao game session player cube
    string initial_cube_state = initCubeState();
    const char* sql = Query::INSERT_GAME_SESSION;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, game_session_id);
    sqlite3_bind_int(stmt, 2, player_id);
    sqlite3_bind_text(stmt, 3, initial_cube_state.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Game session %d started successfully.\n", game_session_id);
    }

    sqlite3_finalize(stmt);
};
//Nhung gi minh can lam trong 3s countdown.

// bool canStartGame() {
//     // check dieu kien (1) du player, (2) everi player readyyy
//     // gui response to client
//     // bat dau startGameSession(game_session_id, player_id, initCubeState())
// };

json Room::canStartGame(){
    for(const auto& participant : participants){
        if(participant.participant_type == "PLAYER" && !participant.is_ready){
            return false;
        }
    }
    return true;
}

// string initCubeState(){
//     //return cube state ban dau
// }

string Room::initCubeState(){
    //INSERT NEW INITIAL CUBE STATE RECORD INTO DATABASE, return it as string format
    return "first cube state goes here";
}