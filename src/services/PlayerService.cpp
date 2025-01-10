nlohmann::json Player::viewRoomList() {
    vector<Room> rooms = server.getRooms();
    nlohmann::json rooms_json = nlohmann::json::array();
    for (const auto& room : rooms) {
        rooms_json.push_back(room.toJson()); // Assuming Room has a toJson() method
    }
    return MessageHandler::craftResponse("success", {{"rooms", rooms_json}});
}

nlohmann::json Player::createRoom(int max_players, int max_spectators, Server& server) {
    // Get the current time using chrono
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);

    // Create a new Room object
    Room room(Server::room_id_counter++, this->id, now_time_t, max_players, max_spectators, RoomStatus::WAITING, server, vector<GameSession>(), 1, 0);
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

