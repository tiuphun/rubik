#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <sqlite3.h>
#include "../include/nlohmann/json.hpp"
#include "../repositories/PlayerRepository.h"
#include "../repositories/AdminRepository.h"
#include "../repositories/AuthRepository.h"
#include "../models/header/EntityManager.h"
#include "../services/AdminService.h"
#include "../services/PlayerService.h"

using json = nlohmann::json;
using namespace std;


class UserService {
public:
    UserService(PlayerRepository& playerRepo, 
                AdminRepository& adminRepo,
                AuthRepository& authRepo, 
                EntityManager& entityManager,
                AdminService& adminService,
                PlayerService& playerService)
        : playerRepo(playerRepo)
        , adminRepo(adminRepo)
        , authRepo(authRepo)
        , entityManager(entityManager)  
        , adminService(adminService)
        , playerService(playerService) {}

    json signUp(const string& username, const string& password);
    json signIn(const string& username, const string& password, int client_socket);

private:
    AdminService& adminService;
    PlayerService& playerService;
    PlayerRepository& playerRepo;
    AdminRepository& adminRepo;
    AuthRepository& authRepo;
    EntityManager& entityManager;
   
};

#endif