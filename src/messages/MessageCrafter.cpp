#include "MessageCrafter.h"
#include <iostream>
using namespace std;

nlohmann::json MessageCrafter::craftResponse(const std::string& status, const nlohmann::json& data) {
    nlohmann::json response = {{"status", status}, {"data", data}};
    // cout << "Crafted response: " << response.dump() << endl; // Debug print
    return response;
}
