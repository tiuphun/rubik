#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <sqlite3.h>
#include "../services/UserService.h"

#define PORT 8080

using namespace std;

class Server {
public:
    Server();                     
    void start();                 
    void handleClient(int client_socket); 
    void processMessage(const string& message, int client_socket);

private:
    int server_socket; 
    sqlite3 *db;
    const char* db_path = "/Volumes/DATA/repository/rubikServer/rubik/src/database/migration/Rubik.db";
    UserService userService;
};

#endif
