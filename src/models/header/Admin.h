#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>

using namespace std;

class Admin {
public:
    int id;
    string username;
    string password_hash;
    time_t created_at;
    time_t last_login; 
    
    void banPlayer(int player_id);
    void viewPlayerList();
    
};

#endif