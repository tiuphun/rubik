#ifndef GAME_SERVICE_H
#define GAME_SERVICE_H

#include "../models/header/EntityManager.h"
#include "../models/header/GameSession.h"
#include "../messages/MessageCrafter.h"
#include <nlohmann/json.hpp>
#include <thread>
#include <atomic>

class GameService {
public:
    GameService(EntityManager& entityManager) 
        : entityManager(entityManager) {}

    nlohmann::json endGameSession(int sessionId, int playerId);
    nlohmann::json updateCubeState(int sessionId, int playerId, const string& newCubeState);
    nlohmann::json getGameState(int sessionId);
    void startPeriodicUpdates(int sessionId, int adminSocketFd, int intervalSeconds);
    void stopPeriodicUpdates();

private:
    EntityManager& entityManager;
    std::thread updateThread;
    std::atomic<bool> stopUpdates;
    bool isValidCubeState(const string& cubeState);
    bool isPlayerInSession(int sessionId, int playerId);
    json sendCubeState(int sessionId, int adminSocketFd);
};

#endif