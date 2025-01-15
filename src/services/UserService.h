#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <sqlite3.h>
#include "../include/nlohmann/json.hpp"
#include "../repositories/PlayerRepository.h"
#include "../repositories/AdminRepository.h"
#include "../repositories/AuthRepository.h"
#include "../models/header/EntityManager.h"
#include "AdminService.h"
#include "PlayerService.h"

using json = nlohmann::json;
using namespace std;

class UserService {
public:
    UserService(PlayerRepository& playerRepo, 
                AdminRepository& adminRepo,
                AuthRepository& authRepo, 
                EntityManager& entityManager)
        : playerRepo(playerRepo)
        , adminRepo(adminRepo)
        , authRepo(authRepo)
        , entityManager(entityManager) 
        // , adminService(adminRepo, playerRepo, entityManager)
        // , playerService(entityManager, playerRepo) {}

    json signUp(const string& username, const string& password);
    json signIn(const string& username, const string& password, int client_socket);

private:
    PlayerRepository& playerRepo;
    AdminRepository& adminRepo;
    AuthRepository& authRepo;
    EntityManager& entityManager;
    // AdminService adminService;
    // PlayerService playerService;
};

#endif