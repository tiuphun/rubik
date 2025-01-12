#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <../../include/nlohmann/json.hpp>
#include <sqlite3.h>

using json = nlohmann::json;

class MessageHandler {
public:
    // MessageHandler(Server& server) : server(server) {}
    static json parseMessage(const std::string& message);
    static json handleMessage(const json& parsed_message, sqlite3* db, int client_socket);
    static json craftResponse(const std::string& status, const json& data);
private:
    // Server& server;

    static json handleSignUp(const json& parsed_message, sqlite3* db);
    static json handleSignIn(const json& parsed_message, sqlite3* db, int client_socket);
    static json handleCreateRoom(const json& parsed_message, sqlite3* db);
    static json handleJoinRoom(const json& parsed_message, sqlite3* db);
    static json handleLeaveRoom(const json& parsed_message, sqlite3* db);
    static json handleReady(const json& parsed_message, sqlite3* db);
    static json handleStartGame(const json& parsed_message, sqlite3* db);
    static json handleCube(const json& parsed_message, sqlite3* db);
    static json handleEndGame(const json& parsed_message, sqlite3* db);
    static json handleViewUsers(const json& parsed_message, sqlite3* db);
    static json handleViewRooms(const json& parsed_message, sqlite3* db);
    static json handleBanPlayer(const json& parsed_message, sqlite3* db);
    static json handleSpectate(const json& parsed_message, sqlite3* db); 
};

#endif