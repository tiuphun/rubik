// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <nlohmann/json.hpp>

extern sqlite3* db;

// Function declarations
void initializeDatabase();
bool registerPlayer(const std::string& username, const std::string& password_hash, const std::string& email);
bool loginPlayer(const std::string& username, const std::string& password_hash);


bool savePlayerStatistics(int playerId, const nlohmann::json& stats);
nlohmann::json fetchRoomDetails(int roomId);

#endif
