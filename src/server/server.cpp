#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include "../database/header/database.h"
#include "../database/queries/Query.h"
#include "Server.h"
#include "../messages/MessageHandler.h"

using namespace std;

#define PORT 8080

Server::Server() : playerRepo(db), adminRepo(db) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "Failed to create socket\n";
        exit(1);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (::bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        cerr << "Bind failed\n";
        exit(1);
    }

    if (listen(server_socket, 3) < 0) {
        cerr << "Listen failed\n";
        exit(1);
    }

    cout << "Server listening on port " << PORT << "...\n";
    
    int rc = sqlite3_open(db_path, &db);
    if(rc != SQLITE_OK){
        cerr << "Database init failed: " << sqlite3_errmsg(db) << "\n";
        close(server_socket);
        exit(1);
    }

    cout << "Database initialization successful!\n"; 
}

sqlite3* Server::getDb() {
    return db;
}

void Server::handleClient(int client_socket) {
    char buffer[1024] = {0};
    string welcome_msg = "Welcome to the Rubik Server! Send your JSON messages.\n";
    send(client_socket, welcome_msg.c_str(), welcome_msg.length(), 0);

    while (true) {
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            cout << "Client disconnected\n";
            close(client_socket);
            break;
        }

        string message(buffer, bytes_read);
        processMessage(message, client_socket);
    }
}

void Server::processMessage(const std::string& message, int client_socket) {
    json parsed_message = MessageHandler::parseMessage(message);
    json response = MessageHandler::handleMessage(parsed_message, db);
    string response_str = response.dump();
    send(client_socket, response_str.c_str(), response_str.length(), 0);
}

void Server::start() {
    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        
        if (client_socket < 0) {
            cerr << "Accept failed\n";
            continue;
        }

        cout << "New client connected\n";
        handleClient(client_socket);
    }
}

void Server::addPlayer(const Player& player) {
    players.push_back(player);
}

void Server::addAdmin(const Admin& admin) {
    admins.push_back(admin);
}

void Server::addRoom(const Room& room) {
    rooms.push_back(room);
}

vector<Player>& Server::getPlayers() {
    return players;
}

vector<Admin>& Server::getAdmins() {
    return admins;
}

vector<Room>& Server::getRooms() {
    return rooms;
}

Room Server::getRoomById(int room_id){
    for (const Room& room: rooms)
    {
        if (room.getId() == room_id)
        {
            return room;
        }
        
    }
    throw runtime_error("Room with id:" + to_string(room_id) + " not found when Player trying to get room");   
    
}