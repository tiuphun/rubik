#include "UserService.h"
#include "../database/queries/Query.h"
#include "../messages/MessageHandler.h"
#include "../models/header/EntityManager.h"
#include "openssl/sha.h"
#include <string>
#include <string.h>


using namespace std;

UserService::UserService(PlayerRepository& playerRepo, 
                        AdminRepository& adminRepo,
                        EntityManager& entityManager)
    : playerRepo(playerRepo), adminRepo(adminRepo), 
        entityManager(entityManager) {}

json UserService::signUp(const string& username, const string& password) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);

    const char* check_user_sql = Query::FIND_AUTH_USER;
    sqlite3_stmt * check_user_stmt;
    int rc = sqlite3_prepare_v2(db, check_user_sql, -1, &check_user_stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }
    sqlite3_bind_text(check_user_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(check_user_stmt, 2, password_hash.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(check_user_stmt) == SQLITE_ROW) {
        sqlite3_finalize(check_user_stmt);
        return MessageHandler::craftResponse("error", {{"message", "Username already exists"}});
    }
    sqlite3_finalize(check_user_stmt);
    

    // Prepare SQL statement
    const char* sql = Query::INSERT_PLAYER;
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
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

    // Prepare SQL statement to check if the user exists in AuthUsers view
    const char* sql = Query::FIND_AUTH_USER;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string user_type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string account_status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        sqlite3_finalize(stmt);

        if (user_type == "PLAYER") {
            if (account_status == "BANNED") {
                return MessageHandler::craftResponse("error", {{"message", "User is banned"}});
            } else if (account_status == "ACTIVE") {
                return MessageHandler::craftResponse("error", {{"message", "User is already online"}});
            } else if (account_status == "INACTIVE") {
                // Update account status to ACTIVE
                const char* update_sql = "UPDATE Player SET status = 'ACTIVE' WHERE id = ?";
                sqlite3_stmt* update_stmt;
                rc = sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, nullptr);
                if (rc != SQLITE_OK) {
                    return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
                }
                sqlite3_bind_int(update_stmt, 1, id);
                rc = sqlite3_step(update_stmt);
                sqlite3_finalize(update_stmt);
                if (rc != SQLITE_DONE) {
                    return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
                }
                
                //SELECT THE PLAYER BY ID and then parse the data to the player object
                Player player = playerRepo.getPlayerById(id);
                //Create Player auto pointer
                auto playerPtr = make_unique<Player>(
                    player.id, 
                    player.username, 
                    player.password_hash, 
                    player.join_date,
                    player.total_games, 
                    player.wins, 
                    player.best_time, 
                    player.avg_time, 
                    player.status, 
                    player.ban_date, 
                    player.ban_by, 
                    player.socket_fd
                );
                entityManager.addPlayer(move(playerPtr));
                return MessageHandler::craftResponse("success", {{"message", "Player signed in successfully"}});
            }
        } else if (user_type == "ADMIN") {
            if (account_status == "BANNED") {
                return MessageHandler::craftResponse("error", {{"message", "User is banned"}});
            } else if (account_status == "ACTIVE") {
                return MessageHandler::craftResponse("error", {{"message", "User is already online"}});
            } else if (account_status == "INACTIVE") {
                // Update account status to ACTIVE
                const char* update_sql = "UPDATE Admin SET status = 'ACTIVE' WHERE id = ?";
                sqlite3_stmt* update_stmt;
                rc = sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, nullptr);
                if (rc != SQLITE_OK) {
                    return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
                }
                sqlite3_bind_int(update_stmt, 1, id);
                rc = sqlite3_step(update_stmt);
                sqlite3_finalize(update_stmt);
                if (rc != SQLITE_DONE) {
                    return MessageHandler::craftResponse("error", {{"message", sqlite3_errmsg(db)}});
                }
                //SELECT THE ADMIN BY ID and then parse the data to the admin object
                Admin admin = adminRepo.getAdminById(id);
                //Add admin to EntityManager ADMIN VECTOR
                auto adminPtr = make_unique<Admin>(
                    admin.id, 
                    admin.username, 
                    admin.password_hash, 
                    admin.created_at, 
                    admin.last_login, 
                    admin.socket_fd
                );
                entityManager.addAdmin(move(adminPtr));
                
                return MessageHandler::craftResponse("success", {{"message", "Admin signed in successfully"}});
            }
        }
    } else {
        sqlite3_finalize(stmt);
        return MessageHandler::craftResponse("error", {{"message", "Invalid username or password"}});
    }
}