#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "../repositories/PlayerRepository.h"
#include "../models/header/Admin.h"
#include <sqlite3.h>

class AdminService {
public:
    AdminService(PlayerRepository& playerRepo)
        : playerRepo(playerRepo) {}

    nlohmann::json banPlayer(int player_id, AdminService &adminService);
    nlohmann::json viewPlayerList(AdminService &adminService);
    nlohmann::json viewRoomList();
    nlohmann::json spectate(int game_session_id, int room_id);
    nlohmann::json leaveGame();

private:
    PlayerRepository& playerRepo;
};

#endif // ADMINSERVICE_H