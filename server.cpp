#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring> // For memset
#include <cstdlib> // For exit()
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h> // For close()
#include <arpa/inet.h> // For inet_ntoa
#include <nlohmann/json.hpp> // Include JSON library

using json = nlohmann::json;
const int PORT = 8080; // Server port
const int BUFFER_SIZE = 1024;

// Function to handle individual client connections
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    // Receive data from the client
    ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
        std::cerr << "Error receiving data from client." << std::endl;
        close(clientSocket);
        return;
    }

    std::cout << "Message received: " << buffer << std::endl;

    // Parse the received message as JSON
    try {
        json receivedData = json::parse(buffer);
        std::cout << "Parsed JSON: " << receivedData.dump(4) << std::endl;

        // Example: Respond to a ping message
        if (receivedData["type"] == "ping") {
            json response = {{"type", "pong"}, {"message", "Hello from server!"}};
            std::string responseStr = response.dump();
            send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
        }
    } catch (json::parse_error &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    // Close the connection
    close(clientSocket);
    std::cout << "Client disconnected." << std::endl;
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Binding failed!" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Listening failed!" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << PORT << "..." << std::endl;

    // Accept incoming connections
    while (true) {
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            std::cerr << "Connection acceptance failed!" << std::endl;
            continue;
        }

        std::cout << "Connected to client: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        // Handle the client in a new thread
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    // Close the server socket
    close(serverSocket);
    return 0;
}
