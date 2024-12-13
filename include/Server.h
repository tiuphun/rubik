#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <netinet/in.h>

#define PORT 8080

using namespace std;

class Server {
public:
    Server();                       // Constructor
    // ~Server();                      // Destructor
    void start();                   // Starts the server
    void handle_client(int client_socket); // Handles client communication

private:
    unordered_map<string, string> load_users(); // Load users from file
    void save_user(const string& username, const string& password); // Save user to file
    void sign_up(int client_socket, const string& username, const string& password); // Sign up logic
    void sign_in(int client_socket, const string& username, const string& password); // Sign in logic

    int server_socket; // Server socket descriptor
};

#endif
