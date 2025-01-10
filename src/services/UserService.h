#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <sqlite3.h>
#include "../include/nlohmann/json.hpp"
#include "../repositories/PlayerRepository.h"
#include "../repositories/AdminRepository.h"
#include "../models/header/EntityManager.h"

using json = nlohmann::json;
using namespace std;

class UserService {
public:
    UserService(PlayerRepository& playerRepo, 
                AdminRepository& adminRepo, 
                EntityManager& entityManager)
        : playerRepo(playerRepo)
        , adminRepo(adminRepo)
        , entityManager(entityManager){}

    json signUp(const string& username, const string& password);
    json signIn(const string& username, const string& password);

private:
    PlayerRepository& playerRepo;
    AdminRepository& adminRepo;
    EntityManager& entityManager;
};

#endif