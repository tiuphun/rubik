#include "AuthRepository.h"
#include "../database/queries/Query.h"
#include <iostream>
using namespace std;

bool AuthRepository::isUsernameTaken(const string& username) {
    const char* check_query = Query::FIND_AUTH_USER_BY_NAME;
    sqlite3_stmt* check_user_exist_stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, check_query, -1, &check_user_exist_stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement to check if username is taken: " 
             << sqlite3_errmsg(db) << endl;
        return false;  
    }

    sqlite3_bind_text(check_user_exist_stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(check_user_exist_stmt);
    if (rc == SQLITE_ROW) { //If username exist.
        sqlite3_finalize(check_user_exist_stmt); 
        return true;
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(check_user_exist_stmt); 
        return false;
    } else {
        cerr << "Error: Failed to execute step to check if username is taken: " 
             << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(check_user_exist_stmt); 
        return false;
    }
}


bool AuthRepository::isPlayerBanned(const string& username) {
    const char* check_query = Query::FIND_AUTH_USER_BY_STATUS;  
    sqlite3_stmt* check_user_banned_stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, check_query, -1, &check_user_banned_stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement to check if player is banned: " 
             << sqlite3_errmsg(db) << endl;
        return false; 
    }

    sqlite3_bind_text(check_user_banned_stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(check_user_banned_stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* account_status_text = sqlite3_column_text(check_user_banned_stmt, 0);
        string account_status = account_status_text ? reinterpret_cast<const char*>(account_status_text) : "";

        sqlite3_finalize(check_user_banned_stmt); 
        return account_status == "BANNED";
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(check_user_banned_stmt); 
        return false;
    } else {
        cerr << "Error: Failed to execute step to check if player is banned: " 
             << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(check_user_banned_stmt); 
        return false;
    }
}


/**
 * Validate the credentials of a user
 * @param username The username of the user
 * @param password The password of the user
 * @return optional struct of the result. If the user is not found, return nullopt
 */
optional<AuthResult> AuthRepository::validateCredentials(const string& username, const string& password) {
    const char* check_query = Query::FIND_AUTH_USER;
    sqlite3_stmt* check_user_stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, check_query, -1, &check_user_stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement for user validation: " 
             << sqlite3_errmsg(db) << endl;
        return nullopt;  
    }

    sqlite3_bind_text(check_user_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(check_user_stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    rc = sqlite3_step(check_user_stmt);
    if (rc == SQLITE_ROW) {
        AuthResult result{
            sqlite3_column_int(check_user_stmt, 0), // id
            reinterpret_cast<const char*>(sqlite3_column_text(check_user_stmt, 2)), 
            reinterpret_cast<const char*>(sqlite3_column_text(check_user_stmt, 3)) // account_status
        };

        sqlite3_finalize(check_user_stmt); 
        return result;
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(check_user_stmt); 
        return nullopt;
    } else {
        cerr << "Error: Failed to execute step for user validation: " 
             << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(check_user_stmt);
        return nullopt;
    }
}