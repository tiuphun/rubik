#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "GameSession.h"
#include "../../database/queries/Query.h"
#include "../../messages/MessageHandler.h"

nlohmann::json GameSession::endGameSession(int player_id) {
    return MessageHandler::craftResponse("success", {{"results", "hello"}}); // Delete this later
}

