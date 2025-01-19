#include "GameService.h"
#include <chrono>

using namespace std::chrono;

nlohmann::json GameService::endGameSession(int sessionId, int playerId) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageCrafter::craftResponse("error", {{"message", "Game session not found"}});
    }

    if (!isPlayerInSession(sessionId, playerId)) {
        return MessageCrafter::craftResponse("error", {{"message", "Player not in this session"}});
    }

    session->end_time = system_clock::to_time_t(system_clock::now());
    session->status = COMPLETED;

    return MessageCrafter::craftResponse("success", {
        {"message", "Game session ended"},
        {"session", session->toJson()}
    });
}

nlohmann::json GameService::updateCubeState(int sessionId, int playerId, const string& newCubeState) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageCrafter::craftResponse("error", {{"message", "Game session not found"}});
    }

    if (!isPlayerInSession(sessionId, playerId)) {
        return MessageCrafter::craftResponse("error", {{"message", "Player not in this session"}});
    }

    if (!isValidCubeState(newCubeState)) {
        return MessageCrafter::craftResponse("error", {{"message", "Invalid cube state"}});
    }

    session->initial_cube_state = newCubeState;
    session->total_moves++;

    return MessageCrafter::craftResponse("success", {
        {"message", "Cube state updated"},
        // {"session", session->toJson()}
    });
}

nlohmann::json GameService::getGameState(int sessionId) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageCrafter::craftResponse("error", {{"message", "Game session not found"}});
    }

    return MessageCrafter::craftResponse("success", {
        {"session", session->toJson()}
    });
}

bool GameService::isValidCubeState(const string& cubeState) {
    // Basic validation - can be expanded
    //Maybe check cubestate length here
    return true;
}

bool GameService::isPlayerInSession(int sessionId, int playerId) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    return session && session->player_id == playerId;
}


json GameService::startPeriodicUpdates(int sessionId, int adminSocketFd, int intervalSeconds) {
    stopUpdates = false;
    updateThread = std::thread([this, sessionId, adminSocketFd, intervalSeconds]() {
        while (!stopUpdates) {
            return sendCubeState(sessionId, adminSocketFd);
            std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
        }
    });
}

void GameService::stopPeriodicUpdates() {
    stopUpdates = true;
    if (updateThread.joinable()) {
        updateThread.join();
    }
}

json GameService::sendCubeState(int sessionId, int adminSocketFd) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageCrafter::craftResponse("error", {{"message", "Game session not found"}});
    }

    nlohmann::json response = MessageCrafter::craftResponse("success", {
        {"message", "Cube state updating..."},
        {"session", session->toJson()}
    });
    return response;
}