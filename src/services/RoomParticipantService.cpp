nlohmann::json RoomParticipant::leaveRoom(Server &server) {
   Room room = server.getRoomById(this->room_id);
   room.removeRoomParticipant(this->id);
   return MessageHandler::craftResponse("success", {{"message", "RoomParticipant: " + to_string(this->id) + " left the room"}});
}

nlohmann::json RoomParticipant::isReady() {
    this->is_ready = true;
    return MessageHandler::craftResponse("success", {{"message", "RoomParticipant: " + to_string(this->id) + " is ready"}});
}