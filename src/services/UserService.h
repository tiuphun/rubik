#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <sqlite3.h>
#include "../include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class UserService {
public:
    UserService(sqlite3* db) : db(db) {}

    json signUp(const string& username, const string& password);
    json signIn(const string& username, const string& password);

private:
    sqlite3* db;
    //Define counter to track player & admin id. 
    //Modify it based on the draft data. 
    //For example, if 3 users are created already, then change this number.
    int playerLatestId = 1;
    int adminLatestId = 10000;
};

#endif