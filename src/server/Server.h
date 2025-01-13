#ifndef SERVER_H
#define SERVER_H

#include "../models/header/EntityManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <sqlite3.h>
#include <vector>
#include "../repositories/AdminRepository.h"
#include "../repositories/PlayerRepository.h"


#define PORT 8080

using namespace std;
class Server {
public:
    Server();                     
    void start();                 
    void handleClient(int client_socket); 
    void processMessage(const string& message, int client_socket);

    sqlite3* getDb(){
        return this->db;
    } 

private:
    unique_ptr<EntityManager> entityManager;
    int server_socket; 
    sqlite3 *db;
    const char* db_path = "/Volumes/DATA/repository/rubikServer/rubik/src/database/migration/Rubik.db";

};

#endif
