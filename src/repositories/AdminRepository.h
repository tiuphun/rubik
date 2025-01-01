#ifndef ADMINREPOSITORY_H
#define ADMINREPOSITORY_H

#include <vector>
#include "../models/header/Admin.h"
#include <sqlite3.h>

using namespace std;

class AdminRepository {
public:
    AdminRepository(sqlite3* db) : db(db) {}

    vector<Admin> getAllAdmins();
    Admin getAdminById(int id);
    bool insertAdmin(const Admin& admin);

private:
    sqlite3* db;
};

#endif