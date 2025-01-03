#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "GameSession.h"
#include "../../database/queries/Query.h"
#include "../../messages/MessageHandler.h"

nlohmann::json GameSession::createPlayerGameSessions(const vector<int>& playerIds, int cube_state_id) {
    nlohmann::json response_data = json::array();
    for (int player_id : playerIds) {
        const char* sql = Query::INSERT_PLAYER_GAME_SESSION;
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
        }

        sqlite3_bind_int(stmt, 1, this->id);
        sqlite3_bind_int(stmt, 2, player_id);
        sqlite3_bind_int(stmt, 3, cube_state_id);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
        }

        int player_game_session_id = sqlite3_last_insert_rowid(db);
        PlayerGameSession player_game_session(db);
        player_game_session.id = player_game_session_id;
        player_game_session.game_session_id = id;
        player_game_session.player_id = player_id;
        player_game_session.status = "PLAYING";
        player_game_sessions.push_back(player_game_session); // Push back to vector

        response_data.push_back({{"player_game_session_id", player_game_session_id}, {"player_id", player_id}});
        sqlite3_finalize(stmt);
    }
    return MessageHandler::craftResponse("success", {{"player_game_sessions", response_data}});
}

nlohmann::json GameSession::endGameSession(int player_id) {
    /*
    const char* sql = Query::END_GAME_SESSION;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, player_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);

    // Fetch game results
    const char* result_sql = Query::SELECT_GAME_RESULTS;
    rc = sqlite3_prepare_v2(db, result_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    json results = json::array();
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        int completion_time = sqlite3_column_int(stmt, 2);
        results.push_back({{"id", id}, {"username", std::string(reinterpret_cast<const char*>(username))}, {"completion_time", completion_time}});
    }

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"results", results}});
    */
    
    return MessageHandler::craftResponse("success", {{"results", "hello"}}); // Delete this later
}
