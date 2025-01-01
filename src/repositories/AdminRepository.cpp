#include "AdminRepository.h"
#include "../database/queries/Query.h"
#include <vector>


using namespace std;

vector<Admin> AdminRepository::getAllAdmins() {
    vector<Admin> admins;
    const char* sql = Query::SELECT_ALL_ADMIN;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return admins;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        time_t created_at = sqlite3_column_int64(stmt, 3);
        time_t last_login = sqlite3_column_int64(stmt, 4);

        admins.emplace_back(id, username, password_hash, created_at, last_login);
    }
    sqlite3_finalize(stmt);
    return admins;
}

Admin AdminRepository::getAdminById(int id) {
    Admin admin(0, "", "", 0, 0);
    const char* sql = Query::SELECT_ADMIN_BY_ID;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return admin;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        time_t created_at = sqlite3_column_int64(stmt, 3);
        time_t last_login = sqlite3_column_int64(stmt, 4);

        admin = Admin(id, username, password_hash, created_at, last_login);
    }
    sqlite3_finalize(stmt);
    return admin;
}

bool AdminRepository::insertAdmin(const Admin& admin) {
    const char* sql = Query::INSERT_ADMIN;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, admin.id);
    sqlite3_bind_text(stmt, 2, admin.username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, admin.password_hash.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}