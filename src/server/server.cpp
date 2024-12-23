#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sqlite3.h>
#include "../database/header/database.h"
#include "../database/queries/Query.h"

using namespace std;

#define PORT 8080

class Server {
public:
    Server();
    sqlite3 *db; //Database holder
    const char* db_path = "/Volumes/DATA/repository/rubikServer/rubik/src/database/migration/Rubik.db"; //Absolute path
    void start();
    void handle_client(int client_socket);
    
private:

    unordered_map<string, string> load_users();
    void save_user(const string& username, const string& password);

    void sign_up(int client_socket, const string& username, const string& password);
    void sign_in(int client_socket, const string& username, const string& password);
    
    int server_socket;
};

Server::Server() {


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
    /* Database init after socket init:
    */
    cout << "Loading Database..." << "\n";
    cout << "Database path:" << db_path << "\n";
    
    int rc = Database::db_init(db_path, &db);
    if(rc != SQLITE_OK){
        cerr << "Database init failed\n";
        close(server_socket);
        exit(1);
    }

    cout << "Database initialization successful!\n"; 
}

void Server::sign_up(int client_socket, const string& username, const string& password) {
    
}

void Server::sign_in(int client_socket, const string& username, const string& password) {
   
}

void Server::handle_client(int client_socket) {
    char buffer[1024] = {0};
    string welcome_msg = "Welcome to the Rubik Server! Type SIGNUP <username> <password> to register or SIGNIN <username> <password> to login.\n";
    send(client_socket, welcome_msg.c_str(), welcome_msg.length(), 0);

    while (true) {
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            cout << "Client disconnected\n";
            close(client_socket);
            break;
        }

        string message(buffer, bytes_read);
        message.erase(message.find_last_not_of(" \n\r\t") + 1); // Trim whitespace

        cout << "Received: " << message << endl;

        if (message.substr(0, 7) == "BAN_PLAYER_REQUEST") {
            istringstream ss(message.substr(7));
            string username, password;
            ss >> username >> password;
            sign_up(client_socket, username, password);
        } else if (message.substr(0, 6) == "SIGNIN ") {
            istringstream ss(message.substr(6));
            string username, password;
            ss >> username >> password;
            sign_in(client_socket, username, password);
        } else {
            string unknown = "Unknown command. Try SIGNUP <username> <password> or SIGNIN <username> <password>.\n";
            send(client_socket, unknown.c_str(), unknown.length(), 0);
        }
    }
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
        handle_client(client_socket);
    }
}