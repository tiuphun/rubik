#ifndef GAME_SERVICE_H
#define GAME_SERVICE_H

#include "../models/header/EntityManager.h"
#include "../models/header/GameSession.h"
#include "../messages/MessageHandler.h"
#include <nlohmann/json.hpp>

class GameService {
public:
    GameService(EntityManager& entityManager) 
        : entityManager(entityManager) {}

    nlohmann::json endGameSession(int sessionId, int playerId);
    nlohmann::json updateCubeState(int sessionId, int playerId, const string& newCubeState);
    nlohmann::json getGameState(int sessionId);

private:
    EntityManager& entityManager;
    bool isValidCubeState(const string& cubeState);
    bool isPlayerInSession(int sessionId, int playerId);
};

#endif