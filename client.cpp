#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define SERVER_IP "127.0.0.1" // Change to your server's IP if not local
#define SERVER_PORT 8080      // Change to match your server's port

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Create the socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error: Unable to create socket\n";
        return 1;
    }

    // Set up the server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address\n";
        close(clientSocket);
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error: Connection to the server failed\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to the server!\n";

    // Create a sample JSON message
    json request;
    request["type"] = "ping";  // Example message type
    request["message"] = "Hello, server!";

    std::string requestStr = request.dump();

    // Send the message to the server
    if (send(clientSocket, requestStr.c_str(), requestStr.length(), 0) < 0) {
        std::cerr << "Error: Failed to send message\n";
        close(clientSocket);
        return 1;
    }

    std::cout << "Message sent: " << requestStr << "\n";

    // Receive the server's response
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0) {
        std::cerr << "Error: Failed to receive response\n";
        close(clientSocket);
        return 1;
    }

    // Print the server's response
    std::cout << "Server response: " << buffer << "\n";

    // Close the socket
    close(clientSocket);

    return 0;
}
