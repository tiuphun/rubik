#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "GameSession.h"
#include "../../database/queries/Query.h"

void GameSession::createPlayerGameSessions(const vector <int>& playerIds, int cube_state_id) {
    //Tao ra n PlayerGameSession = n player_id.
    //Tao ra 1 CubeState ban dau
    for (int player_id : playerIds)
    {
       const char* sql = Query::INSERT_PLAYER_GAME_SESSION;
       sqlite3_stmt *stmt;
       int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            continue;
        }

        sqlite3_bind_int(stmt, 1, this->id);
        sqlite3_bind_int(stmt, 2, player_id);
        sqlite3_bind_int(stmt, 3, cube_state_id);

        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        } else {
            int player_game_session_id = sqlite3_last_insert_rowid(db);
            PlayerGameSession player_game_session(db);
            player_game_session.id = player_game_session_id;
            player_game_session.game_session_id = id;
            player_game_session.player_id = player_id;
            player_game_session.status = "PLAYING";
            player_game_sessions.push_back(player_game_session); //Push back to vector

            printf("Player game session with id: %d created for player with id: %d.\n", player_game_session_id, player_id);
        }

        sqlite3_finalize(stmt);

    }
    
}

void endGameSession(int player_id) {
    // if winner_id not null -> game end luon
    // other player an cuc :V
    // display result: Server select RoomPlayers & PlayerGameSession 
    // De lay ra id, username & completion_time
    // Send to client, client process.
};
