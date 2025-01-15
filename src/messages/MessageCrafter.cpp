#include "MessageCrafter.h"

class MessageCrafter {
public:
    static nlohmann::json craftResponse(const std::string& status, const nlohmann::json& data) {
        return {{"status", status}, {"data", data}};
    }
};
