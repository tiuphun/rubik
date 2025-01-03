#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <sqlite3.h>
#include "../services/UserService.h"
#include "../models/header/Player.h"
#include "../models/header/Admin.h"
#include "../models/header/Room.h"
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
    
    //Handle vectors of entity data
    void addPlayer(const Player& player);
    void addAdmin(const Admin& admin);
    void addRoom(const Room& Room);

    vector<Player>& getPlayers();
    vector<Admin>& getAdmins();
    vector<Room>& getRooms();

    Room getRoomById(int room_id);
    Admin getAdminById(int admin_id);
    Player getPlayerById(int player_id);

    sqlite3* getDb(); // Public getter for db

    static int room_id_counter;
    static int game_session_id_counter;
    static int player_game_session_id_counter;

private:
    int server_socket; 
    sqlite3 *db;
    const char* db_path = "/Volumes/DATA/repository/rubikServer/rubik/src/database/migration/Rubik.db";
    //Repository helper classes to fetch user data:
    PlayerRepository playerRepo;
    AdminRepository adminRepo;

    vector<Player> players;
    vector<Admin> admins;
    vector<Room> rooms;

};

#endif
