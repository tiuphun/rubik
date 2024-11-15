// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <nlohmann/json.hpp>

bool savePlayerStatistics(int playerId, const nlohmann::json& stats);
nlohmann::json fetchRoomDetails(int roomId);

#endif
