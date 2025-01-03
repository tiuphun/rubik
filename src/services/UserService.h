#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <sqlite3.h>
#include "../include/nlohmann/json.hpp"
#include "../repositories/PlayerRepository.h"
#include "../repositories/AdminRepository.h"
#include "../server/Server.h"

//Forward declaration
class Server;
using json = nlohmann::json;
using namespace std;

class UserService {
public:
    UserService(PlayerRepository& playerRepo, AdminRepository& adminRepo, Server& server);

    PlayerRepository playerRepo;
    AdminRepository adminRepo;
    Server& server;

    json signUp(const string& username, const string& password);
    json signIn(const string& username, const string& password);

private:
    sqlite3* db;
    
};

#endif