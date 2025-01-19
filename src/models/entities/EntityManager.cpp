#include "EntityManager.h"
#include <algorithm>

using namespace std;

int EntityManager::roomIdCounter = 1;
int EntityManager::gameSessionIdCounter = 1;

void EntityManager::addPlayer(unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

void EntityManager::addAdmin(unique_ptr<Admin> admin) {
    admins.push_back(std::move(admin));
}

void EntityManager::addRoom(unique_ptr<Room> room) {
    rooms.push_back(std::move(room));
}

void EntityManager::addRoomParticipant(unique_ptr<RoomParticipant> rp) {
    int roomId = rp->room_id;
    int participantId = rp->id;
    // Add to relationship map
    roomToParticipants[roomId].push_back(participantId);
    //Add to entities vector
    participants.push_back(std::move(rp));
}

void EntityManager::addGameSession(unique_ptr<GameSession> gs) {
    int roomId = gs->room_id;
    int sessionId = gs->id;
    roomToGameSessions[roomId].push_back(sessionId);
    gameSessions.push_back(std::move(gs));
}

Player* EntityManager::getPlayerById(int id) {
    auto it = std::find_if(players.begin(), players.end(),
        [id](const unique_ptr<Player>& player) { return player->id == id; });
    return it != players.end() ? it->get() : nullptr;
}

Admin* EntityManager::getAdminById(int id) {
    auto it = std::find_if(admins.begin(), admins.end(),
        [id](const unique_ptr<Admin>& admin) { return admin->id == id; });
    return it != admins.end() ? it->get() : nullptr;
}

Room* EntityManager::getRoomById(int id) {
    auto it = std::find_if(rooms.begin(), rooms.end(),
        [id](const unique_ptr<Room>& room) { return room->id == id; });
    return it != rooms.end() ? it->get() : nullptr;
}

const vector<unique_ptr<Player>>& EntityManager::getAllPlayers() const {
    return players;
}

const vector<unique_ptr<Admin>>& EntityManager::getAllAdmins() const {
    return admins;
}

const vector<unique_ptr<Room>>& EntityManager::getAllRooms() const {
    return rooms;
}

const vector<unique_ptr<GameSession>>& EntityManager::getAllGameSession() const {
    return gameSessions;
}

const vector<unique_ptr<RoomParticipant>>& EntityManager::getAllRoomParticipant() const {
    return participants;
}



RoomParticipant* EntityManager::getRoomParticipantById(int id) {
    auto it = std::find_if(participants.begin(), participants.end(),
        [id](const unique_ptr<RoomParticipant>& rp) { return rp->id == id; });
    return it != participants.end() ? it->get() : nullptr;
}

GameSession* EntityManager::getGameSessionbyId(int id) {
    auto it = std::find_if(gameSessions.begin(), gameSessions.end(),
        [id](const unique_ptr<GameSession>& gs) { return gs->id == id; });
    return it != gameSessions.end() ? it->get() : nullptr;
}

void EntityManager::removePlayer(int id) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [id](const unique_ptr<Player>& player) { return player->id == id; }),
        players.end()
    );
}

void EntityManager::removeAdmin(int id) {
    admins.erase(
        std::remove_if(admins.begin(), admins.end(),
            [id](const unique_ptr<Admin>& admin) { return admin->id == id; }),
        admins.end()
    );
}

void EntityManager::removeRoom(int id) {
    rooms.erase(
        std::remove_if(rooms.begin(), rooms.end(),
            [id](const unique_ptr<Room>& room) { return room->id == id; }),
        rooms.end()
    );
}

void EntityManager::removeRoomParticipant(int id) {
    // Find participant
    auto it = std::find_if(participants.begin(), participants.end(),
        [id](const unique_ptr<RoomParticipant>& rp) { return rp->id == id; });
    
    if (it != participants.end()) {
        int roomId = (*it)->room_id;
        //Remove from map
        auto& participantIds = roomToParticipants[roomId];
        participantIds.erase(
            std::remove(participantIds.begin(), participantIds.end(), id),
            participantIds.end());
        //Remove from vector
        participants.erase(it);
    }
}

void EntityManager::removeGameSession(int id) {
    auto it = std::find_if(gameSessions.begin(), gameSessions.end(),
        [id](const unique_ptr<GameSession>& gs) { return gs->id == id; });
    
    if (it != gameSessions.end()) {
        int roomId = (*it)->room_id;
        auto& sessionIds = roomToGameSessions[roomId];
        sessionIds.erase(
            std::remove(sessionIds.begin(), sessionIds.end(), id),
            sessionIds.end());
        
        gameSessions.erase(it);
    }
}

vector<RoomParticipant*> EntityManager::getRoomParticipants(int roomId) const {
    vector<RoomParticipant*> result;
    
    // Find room's participant IDs in map
    auto mapIt = roomToParticipants.find(roomId);
    if (mapIt != roomToParticipants.end()) {
        // For each participant ID
        for (int participantId : mapIt->second) {
            // Find participant in vector
            auto it = std::find_if(participants.begin(), participants.end(),
                [participantId](const unique_ptr<RoomParticipant>& rp) {
                    return rp->id == participantId;
                });
            if (it != participants.end()) {
                result.push_back(it->get());
            }
        }
    }
    return result;
}

vector<GameSession*> EntityManager::getRoomGameSessions(int roomId) const {
    vector<GameSession*> result;
    
    // Find room's game session IDs in map
    auto mapIt = roomToGameSessions.find(roomId);
    if (mapIt != roomToGameSessions.end()) {
        // For each session ID
        for (int sessionId : mapIt->second) {
            // Find session in vector
            auto it = std::find_if(gameSessions.begin(), gameSessions.end(),
                [sessionId](const unique_ptr<GameSession>& gs) {
                    return gs->id == sessionId;
                });
            if (it != gameSessions.end()) {
                result.push_back(it->get());
            }
        }
    }
    return result;
}