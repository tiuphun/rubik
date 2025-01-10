
nlohmann::json GameSession::endGameSession(int player_id) {
    
    return MessageHandler::craftResponse("success", {{"results", "hello"}}); // Delete this later
}