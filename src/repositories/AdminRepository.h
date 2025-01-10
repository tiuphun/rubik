#ifndef ADMINREPOSITORY_H
#define ADMINREPOSITORY_H

#include <vector>
#include "../models/header/Admin.h"
#include <sqlite3.h>

class Server;

class AdminRepository {
public:
    AdminRepository(sqlite3* db) : db(db) {}
    Admin getAdminById(int id);
    Admin createAdminFromStmt(sqlite3_stmt* stmt);
    

private:
    sqlite3* db;
    
};

#endif