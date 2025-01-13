#include "GameService.h"
#include <chrono>

using namespace std::chrono;

nlohmann::json GameService::endGameSession(int sessionId, int playerId) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageHandler::craftResponse("error", {{"message", "Game session not found"}});
    }

    if (!isPlayerInSession(sessionId, playerId)) {
        return MessageHandler::craftResponse("error", {{"message", "Player not in this session"}});
    }

    session->end_time = system_clock::to_time_t(system_clock::now());
    session->status = COMPLETED;

    return MessageHandler::craftResponse("success", {
        {"message", "Game session ended"},
        {"session", session->toJson()}
    });
}

nlohmann::json GameService::updateCubeState(int sessionId, int playerId, const string& newCubeState) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageHandler::craftResponse("error", {{"message", "Game session not found"}});
    }

    if (!isPlayerInSession(sessionId, playerId)) {
        return MessageHandler::craftResponse("error", {{"message", "Player not in this session"}});
    }

    if (!isValidCubeState(newCubeState)) {
        return MessageHandler::craftResponse("error", {{"message", "Invalid cube state"}});
    }

    session->initial_cube_state = newCubeState;
    session->total_moves++;

    return MessageHandler::craftResponse("success", {
        {"message", "Cube state updated"},
        {"session", session->toJson()}
    });
}

nlohmann::json GameService::getGameState(int sessionId) {
    auto* session = entityManager.getGameSessionbyId(sessionId);
    if (!session) {
        return MessageHandler::craftResponse("error", {{"message", "Game session not found"}});
    }

    return MessageHandler::craftResponse("success", {
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