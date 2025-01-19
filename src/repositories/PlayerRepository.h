#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <vector>
#include "../models/header/Player.h"
#include "Const.h"
#include <sqlite3.h>

class PlayerRepository {
public:
    PlayerRepository(sqlite3* db): db(db) {}
    bool registerPlayer(const string& username, const string& password );
    Player createPlayerFromStmt(sqlite3_stmt* stmt);
    std::vector<Player> getAllPlayers();
    Player getPlayerById(int id);
    bool disconnectPlayerStatusUpdate(int id); //Gọi khi disconnect socket thông qua PlayerRepository -> Update vào db status inactive
    bool connectPlayerStatusUpdate(int id); // Gọi hàm này trong PlayerRepository sau khi fetch player vào EntityManager
    sqlite3* getDb() const { return db; }
private:
    sqlite3* db;
};

#endif 