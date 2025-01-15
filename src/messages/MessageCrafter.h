#ifndef MESSAGE_CRAFTER_H
#define MESSAGE_CRAFTER_H

#include <string>
#include <../../include/nlohmann/json.hpp>
#include <sqlite3.h>
#include "../services/UserService.h"
#include "../services/PlayerService.h"
#include "../services/AdminService.h"
#include "../services/GameService.h"
#include "../services/RoomService.h"
#include "../repositories/AuthRepository.h"
#include "../repositories/PlayerRepository.h"
#include "../repositories/AdminRepository.h"
#include "../models/header/EntityManager.h"

using json = nlohmann::json;

class MessageCrafter {
public:

    static json craftResponse(const std::string& status, const json& data);
private:
};

#endif