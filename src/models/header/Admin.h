#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <sqlite3.h>
#include "../../include/nlohmann/json.hpp"

using namespace std;

class Admin {
public:
    int id;
    string username;
    string password_hash;
    time_t created_at;
    time_t last_login; 
    int socket_fd;
    
    //Default constructor
    Admin() 
        : id(0)
        , username("")
        , password_hash("")
        , created_at(0)
        , last_login(0)
        , socket_fd(-1) {}
    Admin(int id, const string& username, const string& password_hash, time_t created_at, time_t last_login,int socket_fd)
        : id(id), username(username), password_hash(password_hash), created_at(created_at), last_login(last_login), socket_fd(socket_fd) {}
   
};

#endif