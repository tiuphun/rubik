#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "PlayerGameSession.h"
#include "../../database/queries/Query.h"
#include "../../messages/MessageHandler.h"

json PlayerGameSession::updateCubeFromClient(string current_cube) {
    // Parse the current cube state
    // Assuming current_cube is a string representation of the cube state
    // Example: "Y O Y Y Y O Y B B W B G B W B Y R Y R W R W R R G B G O G Y G G O G O R O R O B W O W G W B W R"
    
    // Prepare SQL statement to update the cube state in the database
    const char* sql = Query::INSERT_CUBE_STATE;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    // Bind the current cube state and player game session ID to the SQL statement
    sqlite3_bind_text(stmt, 1, current_cube.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, this->id);

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    // Finalize the SQL statement
    sqlite3_finalize(stmt);

    // Return a success response
    return MessageHandler::craftResponse("success", {{"message", "Cube state updated successfully"}});
}