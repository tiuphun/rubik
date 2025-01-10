#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include "../../include/nlohmann/json.hpp"
#include "../constants/Const.h"


nlohmann::json createRoom(int max_players, int max_spectators);
nlohmann::json joinRoom(int room_id, RoomParticipantStatus participant_type);
nlohmann::json viewRoomList();

#endif