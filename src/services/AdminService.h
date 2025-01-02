#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "../repositories/PlayerRepository.h"
#include "../models/header/Admin.h"
#include <sqlite3.h>

class AdminService {
public:
    AdminService(PlayerRepository& playerRepo)
        : playerRepo(playerRepo) {}

    nlohmann::json banPlayer(int player_id, int admin_id);
    nlohmann::json viewPlayerList();

private:
    PlayerRepository& playerRepo;
};

#endif // ADMINSERVICE_H