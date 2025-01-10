#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <vector>
#include "Player.h"
#include "Const.h"
#include <sqlite3.h>

class PlayerRepository {
public:
    PlayerRepository(sqlite3* db): db(db) {}
    bool registerPlayer(const string& username, const string& password );
    Player createPlayerFromStmt(sqlite3_stmt* stmt);
    std::vector<Player> getAllPlayers();
    Player getPlayerById(int id);

private:
    sqlite3* db;
};

#endif 