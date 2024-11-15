// player_manager.h
#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <string>
#include <nlohmann/json.hpp>

bool authenticatePlayer(const std::string& username, const std::string& password);
nlohmann::json getPlayerStatistics(int playerId);

#endif
