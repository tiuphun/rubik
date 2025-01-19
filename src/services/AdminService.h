#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "../repositories/AdminRepository.h"
#include "../repositories/PlayerRepository.h"
#include "../models/header/EntityManager.h"
#include "../models/header/GameSession.h"
#include "../messages/MessageCrafter.h"
#include <nlohmann/json.hpp>

class AdminService {
public:
    AdminService(AdminRepository& adminRepo, 
                PlayerRepository& playerRepo,
                EntityManager& entityManager)
        : adminRepo(adminRepo)
        , playerRepo(playerRepo)
        , entityManager(entityManager) {}

    // Entity operations
    nlohmann::json banPlayer(int playerId, int adminId);
    nlohmann::json viewPlayerList();
    nlohmann::json viewRoomList();
    nlohmann::json spectate(int gameSessionId, int roomId, int adminSocketFd);
    nlohmann::json leaveGame(int adminId);

    // Admin management
    bool updateAdminSocket(int adminId, int socketFd);
    bool disconnectAdmin(int adminId);

private:
    AdminRepository& adminRepo;
    PlayerRepository& playerRepo;
    EntityManager& entityManager;
    GameService gameService;
};

#endif // ADMINSERVICE_H