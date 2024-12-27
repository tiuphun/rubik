#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <nlohmann/json.hpp>
#include <sqlite3.h>

using json = nlohmann::json;

class MessageHandler {
public:
    static json parseMessage(const std::string& message);
    static json handleMessage(const json& parsed_message, sqlite3* db);
    static json craftResponse(const std::string& status, const json& data);
private:
    static json handleSignUp(const json& parsed_message, sqlite3* db);
    static json handleSignIn(const json& parsed_message, sqlite3* db);
    static json handleCreateRoom(const json& parsed_message, sqlite3* db);
    static json handleJoinRoom(const json& parsed_message, sqlite3* db);
};

#endif