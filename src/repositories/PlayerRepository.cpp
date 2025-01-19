#include "PlayerRepository.h"
#include "../database/queries/Query.h"
#include "../models/header/Player.h"
#include <iostream>

using namespace std;


std::vector<Player> PlayerRepository::getAllPlayers() {
    std::vector<Player> players;
    const char* sql = Query::SELECT_ALL_PLAYER;
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return players;
    }

    try {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            players.push_back(createPlayerFromStmt(stmt));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error fetching players: " << e.what() << std::endl;
    }

    sqlite3_finalize(stmt);
    return players;
}

Player PlayerRepository::getPlayerById(int id) {
    Player player;
    const char* sql = Query::SELECT_PLAYER_BY_ID;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return player;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        player = createPlayerFromStmt(stmt);
        
    }
    sqlite3_finalize(stmt);
    return player;
}

Player PlayerRepository::createPlayerFromStmt(sqlite3_stmt* stmt) {
    int id = sqlite3_column_int(stmt, 0);
    std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    time_t join_date = sqlite3_column_int64(stmt, 3);
    int total_games = sqlite3_column_int(stmt, 4);
    int wins = sqlite3_column_int(stmt, 5);
    float best_time = static_cast<float>(sqlite3_column_double(stmt, 6));
    float avg_time = static_cast<float>(sqlite3_column_double(stmt, 7));
    std::string status_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
    PlayerStatus status = (status_str == "ACTIVE") ? PlayerStatus::ACTIVE :
                          (status_str == "INACTIVE") ? PlayerStatus::INACTIVE : PlayerStatus::BANNED;
    time_t ban_date = sqlite3_column_int64(stmt, 9);
    int ban_by = sqlite3_column_int(stmt, 10);

    return Player(id, username, password_hash, join_date, total_games, wins, best_time, avg_time, status, ban_date, ban_by, -1);
}

/**
 * Function to add a player to the database
 * @return true if success, false if fail
 */
bool PlayerRepository::registerPlayer(const string& username, const string& password_hash ){
    const char* sql = Query::INSERT_PLAYER;
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement to check if username is taken: " 
             << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
/**
 * Function to update the STATUS field in the database
 * Return true if the player has the status field updated, 
 */
bool PlayerRepository::connectPlayerStatusUpdate(int player_id){
    const char* update_sql = Query::UPDATE_PLAYER_STATUS_ACTIVE;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db,update_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt,1,player_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update player status ACTIVE with id: "<< player_id << ". " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return sqlite3_changes(db) > 0;
}

bool PlayerRepository::disconnectPlayerStatusUpdate(int player_id){
    const char* update_sql = Query::UPDATE_PLAYER_STATUS_INACTIVE;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db,update_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt,1,player_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to update player status to INACTIVE with id: "<< player_id << ". " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return sqlite3_changes(db) > 0;
}

