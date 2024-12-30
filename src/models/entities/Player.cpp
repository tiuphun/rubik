#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Player.h"
#include "../../database/queries/Query.h"
#include "Const.h"
#include "../../messages/MessageHandler.h"

json Player::viewRoomList() {
    const char* sql = Query::SELECT_ALL_AVAILABLE_ROOM;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    json rooms = json::array();
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int room_id = sqlite3_column_int(stmt, 0);
        rooms.push_back({{"room_id", room_id}});
    }

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"rooms", rooms}});
}

json Player::createRoom(int max_players, int max_spectators) {
    const char* sql = Query::INSERT_ROOM;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, this->id);
    sqlite3_bind_int(stmt, 2, max_players);
    sqlite3_bind_int(stmt, 3, max_spectators);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    int room_id = sqlite3_last_insert_rowid(db);
    return MessageHandler::craftResponse("success", {{"message", "Room created successfully"}, {"room_id", room_id}});
}

json Player::joinRoom(int room_id, const char* participant_type) {
    const char* sql = Query::INSERT_ROOM_PARTICIPANT;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_text(stmt, 2, participant_type, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, this->id);
    sqlite3_bind_int(stmt, 4, 0); // DEFAULT: NOT READY after joined

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "Player joined the room successfully"}, {"room_id", room_id}});
}

