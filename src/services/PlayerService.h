// PlayerService.h
#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include <memory>
#include "../../include/nlohmann/json.hpp"
#include "../constants/Const.h"
#include "../models/header/EntityManager.h"
#include "../repositories/PlayerRepository.h"
#include "../models/header/Room.h"

class PlayerService {
public:
    PlayerService(EntityManager& entityManager, PlayerRepository& playerRepo) 
        : entityManager(entityManager)
        , playerRepo(playerRepo) {}

    nlohmann::json createRoom(int playerId, int max_players, int max_spectators);
    nlohmann::json joinRoom(int playerId, int room_id, RoomParticipantStatus participant_type);
    nlohmann::json viewRoomList();
    void updatePlayerSocket(int playerId, int socketFd);

private:
    EntityManager& entityManager;
    PlayerRepository& playerRepo;
};

#endif