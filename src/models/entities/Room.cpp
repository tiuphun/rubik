#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Room.h"
#include "Const.h"
#include "../../database/queries/Query.h"
#include "../../messages/MessageHandler.h"

using namespace std;

nlohmann::json Room::startGameSession(int game_session_id, int player_id, string initial_cube_state) {
    string first_cube_state = initCubeState();
    const char* sql = Query::INSERT_GAME_SESSION;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, game_session_id);
    sqlite3_bind_int(stmt, 2, player_id);
    sqlite3_bind_text(stmt, 3, first_cube_state.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "Game session started successfully"}, {"game_session_id", game_session_id}});
}

nlohmann::json Room::canStartGame() {
    for (const auto& participant : participants) {
        if (participant.participant_type == "PLAYER" && !participant.is_ready) {
            return MessageHandler::craftResponse("error", {{"message", "Not all players are ready"}});
        }
    }
    return MessageHandler::craftResponse("success", {{"message", "All players are ready"}});
}

nlohmann::json Room::initCubeState() {
    // INSERT NEW INITIAL CUBE STATE RECORD INTO DATABASE, return it as string format
    return "first cube state goes here";
}