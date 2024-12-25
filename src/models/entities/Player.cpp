#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Player.h"
#include "../../database/queries/Query.h"


int Player::createRoom(int max_players, int max_spectators) {
    const char* sql = Query::INSERT_ROOM;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_int(stmt, 2, max_players);
    sqlite3_bind_int(stmt, 3, max_spectators);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        printf("Room created successfully.\n");
    }

    sqlite3_finalize(stmt);
    return sqlite3_last_insert_rowid(db);
};

void Player::joinRoom(int room_id) {
    const char* sql = Query::INSERT_ROOM_PARTICIPANT;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_text(stmt, 2, "PLAYER", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, id);
    sqlite3_bind_int(stmt, 4, 0);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Player %d joined the room successfully.\n", id);
    }
    sqlite3_finalize(stmt);
};

void Player::isReady(int room_id) {
    const char* sql = Query::UPDATE_ROOM_PARTICIPANT_READY;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Player %d is ready.\n", id);
    }
    sqlite3_finalize(stmt);
};