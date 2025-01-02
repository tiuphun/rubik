#ifndef ADMINREPOSITORY_H
#define ADMINREPOSITORY_H

#include <vector>
#include "../models/header/Admin.h"
#include <sqlite3.h>

class Server;

class AdminRepository {
public:
    AdminRepository(sqlite3* db, int socket_fd,Server &server) : db(db), socket_fd(socket_fd), server(server) {}
    Admin getAdminById(int id);

private:
    sqlite3* db;
    int socket_fd;
    Server& server;

    Admin createAdminFromStmt(sqlite3_stmt* stmt);
};

#endif