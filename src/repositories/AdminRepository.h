#ifndef ADMINREPOSITORY_H
#define ADMINREPOSITORY_H

#include "../models/header/Admin.h"
#include <vector>
#include <sqlite3.h>

class AdminRepository {
public:
    AdminRepository(sqlite3* db) : db(db) {}
    Admin getAdminById(int id);
    Admin createAdminFromStmt(sqlite3_stmt* stmt);
    bool updateAdminLastLogin(int id);
    sqlite3* getDb() const { return db; }
    bool banPlayer(int playerId, int adminId);

private:
    sqlite3* db;
    
};

#endif