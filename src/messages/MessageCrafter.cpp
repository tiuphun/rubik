#include "MessageCrafter.h"

nlohmann::json MessageCrafter::craftResponse(const std::string& status, const nlohmann::json& data) {
    return {{"status", status}, {"data", data}};
}
