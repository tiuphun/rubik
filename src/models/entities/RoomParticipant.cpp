#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "RoomParticipant.h"
#include "../../database/queries/Query.h"
#include "../../messages/MessageHandler.h"

nlohmann::json RoomParticipant::leaveRoom(int room_id) {
   
}

nlohmann::json RoomParticipant::isReady(int room_id) {
    const char* sql = Query::UPDATE_ROOM_PARTICIPANT_READY;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_int(stmt, 2, this->id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "Room Participant is ready"}});
}