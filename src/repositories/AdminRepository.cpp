#include "AdminRepository.h"
#include "../database/queries/Query.h"
#include "../models/header/Admin.h"
#include <iostream>

Admin AdminRepository::getAdminById(int id) {
    Admin admin;
    const char* sql = Query::SELECT_ADMIN_BY_ID;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return admin;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        admin = createAdminFromStmt(stmt);
    }else{
        cerr << "Admin with id" << id << "not found" << endl;
    }
    sqlite3_finalize(stmt);
    return admin;
}

Admin AdminRepository::createAdminFromStmt(sqlite3_stmt* stmt) {
    int id = sqlite3_column_int(stmt, 0);
    std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    time_t created_at = sqlite3_column_int64(stmt, 3);
    time_t last_login = sqlite3_column_int64(stmt, 4);

    return Admin(id, username, password_hash, created_at, last_login, -1);
}

bool AdminRepository::updateAdminLastLogin(int admin_id) {
    const char* sql = Query::UPDATE_ADMIN_LAST_LOGIN;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, "datetime('now')", -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, admin_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update admin last login: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return sqlite3_changes(db) > 0;
}

bool AdminRepository::banPlayer(int playerId, int adminId) {
    const char* sql = Query::BAN_PLAYER;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare ban player statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, adminId);
    sqlite3_bind_int(stmt, 2, playerId);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to ban player: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return sqlite3_changes(db) > 0;
}