#include "database.h"
#include <sqlite3.h>
#include <iostream>

sqlite3* db = nullptr;

void initializeDatabase() {
    int rc = sqlite3_open("game.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createTablesSQL = R"(
        CREATE TABLE IF NOT EXISTS players (
            player_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            email TEXT NOT NULL,
            join_date DATETIME DEFAULT CURRENT_TIMESTAMP,
            total_games_played INTEGER DEFAULT 0,
            wins INTEGER DEFAULT 0,
            best_time REAL DEFAULT 0.0,
            average_time REAL DEFAULT 0.0,
            status TEXT DEFAULT 'active'
        );

        CREATE TABLE IF NOT EXISTS rooms (
            room_id INTEGER PRIMARY KEY AUTOINCREMENT,
            created_by INTEGER NOT NULL,
            status TEXT DEFAULT 'waiting',
            start_time DATETIME,
            end_time DATETIME,
            player_count INTEGER DEFAULT 0,
            winner_id INTEGER,
            FOREIGN KEY(created_by) REFERENCES players(player_id),
            FOREIGN KEY(winner_id) REFERENCES players(player_id)
        );

        CREATE TABLE IF NOT EXISTS game_sessions (
            session_id INTEGER PRIMARY KEY AUTOINCREMENT,
            room_id INTEGER NOT NULL,
            player_id INTEGER NOT NULL,
            start_time DATETIME DEFAULT CURRENT_TIMESTAMP,
            end_time DATETIME,
            total_moves INTEGER DEFAULT 0,
            time_taken REAL,
            completed BOOLEAN DEFAULT 0,
            FOREIGN KEY(room_id) REFERENCES rooms(room_id),
            FOREIGN KEY(player_id) REFERENCES players(player_id)
        );

        CREATE TABLE IF NOT EXISTS move_history (
            move_id INTEGER PRIMARY KEY AUTOINCREMENT,
            session_id INTEGER NOT NULL,
            player_id INTEGER NOT NULL,
            move TEXT NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(session_id) REFERENCES game_sessions(session_id),
            FOREIGN KEY(player_id) REFERENCES players(player_id)
        );
    )";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, createTablesSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Database initialized successfully!" << std::endl;
    }
}

bool registerPlayer(const std::string& username, const std::string& password_hash, const std::string& email) {
    const char* sql = "INSERT INTO players (username, password_hash, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool loginPlayer(const std::string& username, const std::string& password_hash) {
    const char* sql = "SELECT COUNT(*) FROM players WHERE username = ? AND password_hash = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_TRANSIENT);

    bool loginSuccess = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        loginSuccess = (count > 0);
    }

    sqlite3_finalize(stmt);
    return loginSuccess;
}
