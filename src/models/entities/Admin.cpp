#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Admin.h"
#include "../../database/queries/Query.h"
#include "Const.h"
#include "../../messages/MessageHandler.h"

nlohmann::json Admin::banPlayer(int player_id) {
    const char* sql = Query::BAN_PLAYER;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, this->id); // ban_by = admin_id
    sqlite3_bind_int(stmt, 2, player_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "Player banned successfully"}});
}

nlohmann::json Admin::viewPlayerList() {
    const char* sql = Query::SELECT_ALL_PLAYER;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    json players = json::array();
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        players.push_back({{"id", id}, {"username", std::string(reinterpret_cast<const char*>(username))}});
    }

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"players", players}});
}

nlohmann::json Admin::viewRoomList() {
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


nlohmann::json Admin::spectate(int game_session_id, int room_id) {
    // Insert admin as a spectator into RoomParticipant
    const char* insert_sql = Query::INSERT_ROOM_PARTICIPANT;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, room_id);
    sqlite3_bind_text(stmt, 2, "ADMIN_SPECTATOR", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, this->id); // participant_id = admin_id
    sqlite3_bind_int(stmt, 4, 1); // is_ready = true

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);

    // Check the status of the room
    const char* check_status_sql = Query::CHECK_ROOM_STATUS;
    rc = sqlite3_prepare_v2(db, check_status_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, room_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* status = sqlite3_column_text(stmt, 0);
        if (strcmp((const char*)status, "IN_PROGRESS") == 0) {
            // Logic to get the cube of players
            // const char* get_cube_sql = Query::GET_PLAYER_CUBE;
            // sqlite3_stmt* cube_stmt;
            // rc = sqlite3_prepare_v2(db, get_cube_sql, -1, &cube_stmt, nullptr);
            // if (rc != SQLITE_OK) {
            //     fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            //     sqlite3_finalize(stmt);
            //     return;
            // }

            // // Assuming you have a way to get player IDs in the room
            // // For each player_id in the room
            // int player_id = 0; // Replace with actual player ID
            // sqlite3_bind_int(cube_stmt, 1, player_id);

            // rc = sqlite3_step(cube_stmt);
            // if (rc == SQLITE_ROW) {
            //     const unsigned char* cube = sqlite3_column_text(cube_stmt, 0);
            //     printf("Player %d's cube: %s\n", player_id, cube);
            // }

            // sqlite3_finalize(cube_stmt);
        }
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "Admin spectating the game"}});
}

nlohmann::json Admin::leaveGame(int room_id) {
    const char* sql = Query::DELETE_FROM_ROOM_PARTICIPANT;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_int(stmt, 1, room_id); // Assuming 'id' is the admin's ID
    sqlite3_bind_int(stmt, 2, id); // Assuming 'id' is the admin's ID

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "Admin left the game successfully"}});
}