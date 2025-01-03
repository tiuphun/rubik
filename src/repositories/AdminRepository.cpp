#include "AdminRepository.h"
#include "../database/queries/Query.h"
#include <vector>
#include "../models/header/Admin.h"


using namespace std;

Admin AdminRepository::getAdminById(int id) {
    Admin admin;
    const char* sql = Query::SELECT_ADMIN_BY_ID;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return admin;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        time_t created_at = sqlite3_column_int64(stmt, 3);
        time_t last_login = sqlite3_column_int64(stmt, 4);

        admin = Admin(id, username, password_hash, created_at, last_login);
    }
    sqlite3_finalize(stmt);
    return admin;
}