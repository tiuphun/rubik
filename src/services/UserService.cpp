#include "UserService.h"
#include "../database/queries/Query.h"
#include "openssl/sha.h"
#include "../messages/MessageHandler.h"

json UserService::signUp(const string& username, const string& password) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);

    // Prepare SQL statement
    const char* sql = Query::INSERT_PLAYER;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("success", {{"message", "User registered successfully"}});
}

json UserService::signIn(const string& username, const string& password) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);

    // Prepare SQL statement
    const char* sql = Query::SELECT_PLAYER_BY_USERNAME;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* db_password_hash = sqlite3_column_text(stmt, 2);
        if (password_hash == reinterpret_cast<const char*>(db_password_hash)) {
            sqlite3_finalize(stmt);
            return MessageHandler::craftResponse("success", {{"message", "User signed in successfully"}});
        }
    }

    sqlite3_finalize(stmt);
    return MessageHandler::craftResponse("error", {{"message", "Invalid username or password"}});
}