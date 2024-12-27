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
};

#endif