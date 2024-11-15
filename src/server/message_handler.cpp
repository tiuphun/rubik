// message_handler.cpp
#include "message_handler.h"

std::string handleMessage(const nlohmann::json& request) {
    nlohmann::json response;
    try {
        if (request["type"] == "ping") {
            response["type"] = "pong";
            response["message"] = "Hello from server!";
        } else if (request["type"] == "join_room") {
            // Handle join room logic here
            response["type"] = "join_room_ack";
            response["status"] = "success";
        }
    } catch (...) {
        response["type"] = "error";
        response["message"] = "Invalid message format.";
    }
    return response.dump();
}
