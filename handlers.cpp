#include "handlers.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <unordered_map>

using json = nlohmann::json;

// Simulated database for demonstration
std::unordered_map<std::string, std::string> userDatabase;

std::string handleRequest(const std::string& request) {
    json req = json::parse(request);
    json res;

    if (req["type"] == "register") {
        std::string username = req["username"];
        std::string password = req["password"];
        if (userDatabase.count(username)) {
            res["status"] = "error";
            res["message"] = "Username already exists";
        } else {
            userDatabase[username] = password; // Simplified; hash the password in production
            res["status"] = "success";
            res["message"] = "Registration successful";
        }
    } else if (req["type"] == "login") {
        std::string username = req["username"];
        std::string password = req["password"];
        if (userDatabase.count(username) && userDatabase[username] == password) {
            res["status"] = "success";
            res["message"] = "Login successful";
        } else {
            res["status"] = "error";
            res["message"] = "Invalid username or password";
        }
    } else if (req["type"] == "ping") {
        res["type"] = "pong";
        res["message"] = "Hello from server!";
    } else {
        res["status"] = "error";
        res["message"] = "Invalid request type";
    }

    return res.dump();
}
