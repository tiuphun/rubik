#ifndef ADMINREPOSITORY_H
#define ADMINREPOSITORY_H

#include <vector>
#include "../models/header/Admin.h"
#include <sqlite3.h>

using namespace std;

class Admin;
class AdminRepository {
public:
    AdminRepository(sqlite3* db) : db(db) {}
    Admin getAdminById(int id);

private:
    sqlite3* db;
};

#endif