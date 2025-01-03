#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Player.h"
#include "../../database/queries/Query.h"
#include "Const.h"
#include "../../messages/MessageHandler.h"
#include "../../include/nlohmann/json.hpp"
#include "../../server/Server.h"

nlohmann::json Player::viewRoomList(Server& server) {
    vector<Room> rooms = server.getRooms();
    return MessageHandler::craftResponse("success", {{"rooms", rooms}}); //Change this to return JSON with room list.
}

nlohmann::json Player::createRoom(int max_players, int max_spectators, Server& server) {
    Room room(0, this->id, time(nullptr), max_players, max_spectators, RoomStatus::WAITING);
    server.addRoom(room);
    return MessageHandler::craftResponse("success", {{"message", "Room created successfully"}, {"room_id", room.id}});
}

nlohmann::json Player::joinRoom(int room_id, RoomParticipantStatus participant_type, Server& server) {
    Room joiningRoom = server.getRoomById(room_id);
    if (joiningRoom.status != RoomStatus::WAITING) {
        return MessageHandler::craftResponse("error", {{"message", "Room with id: " + to_string(room_id) + "does not have WAITING state"}});
    }else{
        if (participant_type == RoomParticipantStatus::PLAYER) {
            RoomParticipant participant(room_id, RoomParticipantStatus::PLAYER, this->id, false);
            joiningRoom.addRoomParticipant(participant);
            return MessageHandler::craftResponse("success", {{"message", "Player joined the room as Player role successfully"}, {"room_id", room_id}});
        } else if (participant_type == RoomParticipantStatus::PLAYER_SPECTATOR) {
            RoomParticipant participant(room_id, RoomParticipantStatus::PLAYER_SPECTATOR, this->id, false);
            joiningRoom.addRoomParticipant(participant);
            return MessageHandler::craftResponse("success", {{"message", "Player joined the room as Player Spectator role successfully"}, {"room_id", room_id}});   
        } else {
            return MessageHandler::craftResponse("error", {{"message", "Invalid participant type"}});
        }
    }
}

