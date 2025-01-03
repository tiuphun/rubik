#include "PlayerRepository.h"
#include "../database/queries/Query.h"
#include "../models/header/Player.h"

using namespace std;

vector<Player> PlayerRepository::getAllPlayers() {
    vector<Player> players;
    const char* sql = Query::SELECT_ALL_PLAYER;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return players;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        time_t join_date = sqlite3_column_int64(stmt, 3);
        int total_games = sqlite3_column_int(stmt, 4);
        int wins = sqlite3_column_int(stmt, 5);
        float best_time = static_cast<float>(sqlite3_column_double(stmt, 6));
        float avg_time = static_cast<float>(sqlite3_column_double(stmt, 7));
        string status_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        PlayerStatus status = (status_str == "ACTIVE") ? PlayerStatus::ACTIVE :
                              (status_str == "INACTIVE") ? PlayerStatus::INACTIVE : PlayerStatus::BANNED;
        time_t ban_date = sqlite3_column_int64(stmt, 9);
        int ban_by = sqlite3_column_int(stmt, 10);

        players.emplace_back(id, username, password_hash, join_date, total_games, wins, best_time, avg_time, status, ban_date, ban_by);
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
        return player;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        time_t join_date = sqlite3_column_int64(stmt, 3);
        int total_games = sqlite3_column_int(stmt, 4);
        int wins = sqlite3_column_int(stmt, 5);
        float best_time = static_cast<float>(sqlite3_column_double(stmt, 6));
        float avg_time = static_cast<float>(sqlite3_column_double(stmt, 7));
        string status_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        PlayerStatus status = (status_str == "ACTIVE") ? PlayerStatus::ACTIVE :
                              (status_str == "INACTIVE") ? PlayerStatus::INACTIVE : PlayerStatus::BANNED;
        time_t ban_date = sqlite3_column_int64(stmt, 9);
        int ban_by = sqlite3_column_int(stmt, 10);

        player = Player(id, username, password_hash, join_date, total_games, wins, best_time, avg_time, status, ban_date, ban_by);
    }
    sqlite3_finalize(stmt);
    return player;
}