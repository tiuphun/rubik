#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "RoomParticipant.h"
#include "../../database/queries/Query.h"


void RoomParticipant::leaveRoom() {
    const char* sql = Query::DELETE_FROM_ROOM_PARTICIPANT;
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
        printf("Participant %d left the room successfully.\n", participant_id);
    }

    sqlite3_finalize(stmt);
}