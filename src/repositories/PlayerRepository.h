#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <vector>
#include "../models/header/Player.h"
#include <sqlite3.h>

class Server;

class PlayerRepository {
public:
    PlayerRepository(sqlite3* db, int socket_fd, Server& server)
        : db(db), socket_fd(socket_fd), server(server) {}

    std::vector<Player> getAllPlayers();
    Player getPlayerById(int id);

private:
    sqlite3* db;
    int socket_fd;
    Server& server;

    Player createPlayerFromStmt(sqlite3_stmt* stmt);
};

#endif // PLAYERREPOSITORY_H