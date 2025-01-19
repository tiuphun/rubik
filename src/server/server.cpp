#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <mutex>
#include "../database/header/database.h"
#include "../database/queries/Query.h"
#include "Server.h"


using namespace std;

#define PORT 8080


Server::Server(){
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int rc = sqlite3_open(db_path, &db);
    messageHandler = std::make_unique<MessageHandler>(db);
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
}

void Server::handleClient(int client_socket) {
    // sqlite3* client_db;
    // int rc = sqlite3_open(db_path, &client_db);
    // if(rc != SQLITE_OK) {
    //     cerr << "Client database connection failed: " << sqlite3_errmsg(client_db) << endl;
    //     close(client_socket);
    //     return;
    // }

    // messageHandler = std::make_unique<MessageHandler>(client_db);
    char buffer[1024] = {0};
    // string welcome_msg = "Welcome to the Rubik Server! Send your JSON messages.\n";
    // send(client_socket, welcome_msg.c_str(), welcome_msg.length(), 0);

    while (true) {
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            cout << "Client disconnected\n";
            close(client_socket);
            break;
        }

        string message(buffer, bytes_read);
        cout << "Received message: " << message << endl; // Debug print
        processMessage(message, client_socket);
        memset(buffer,0,sizeof(buffer));
    }

    //sqlite3_close(client_db);
}

void Server::processMessage(const std::string& message, int client_socket) {
    cout << "Processing message\n" << endl;
    nlohmann::json parsed_message = messageHandler->parseMessage(message);
    cout << "Parsed message: " << parsed_message.dump() << endl; // Debug print
    //json response = MessageHandler::handleMessage(parsed_message, db, client_socket);
    json response = messageHandler->handleMessage(parsed_message,db,client_socket);
    cout << "Handled message\n" << endl;
    string response_str = response.dump();
    cout << "Request response: " << response_str << endl; // Debug print
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

        pid_t pid = fork();
        if (pid < 0) {
            cerr << "Fork failed\n";
            close(server_socket);
            continue;
        }
        if (pid == 0) {
            close(server_socket);

            sqlite3* child_db;
            int rc = sqlite3_open(db_path, &child_db);
            if (rc != SQLITE_OK) {
                cerr << "Child process database connection failed: " << sqlite3_errmsg(child_db) << endl;
                close(client_socket);
                exit(1);
            }

            handleClient(client_socket);
            close(client_socket);
            exit(0);
        } else {
            close(client_socket);
            waitpid(-1, nullptr, WNOHANG);
        }
        handleClient(client_socket);
    }
}
