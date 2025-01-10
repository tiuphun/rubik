#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <vector>
#include "../models/header/Player.h"
#include <sqlite3.h>

using namespace std;

class PlayerRepository {
public:
    PlayerRepository(sqlite3* db): db(db) {}

    vector<Player> getAllPlayers();
    Player getPlayerById(int id);

private:
    sqlite3* db;
    Player createPlayerFromStmt(sqlite3_stmt* stmt);
};

#endif 