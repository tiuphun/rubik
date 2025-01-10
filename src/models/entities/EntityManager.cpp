#include "EntityManager.h"
#include <algorithm>


int EntityManager::roomIdCounter = 1;
int EntityManager::gameSessionIdCounter = 1;

void EntityManager::addPlayer(unique_ptr<Player> player) {
    players.push_back(move(player));
}

void EntityManager::addAdmin(unique_ptr<Admin> admin) {
    admins.push_back(move(admin));
}

void EntityManager::addRoom(unique_ptr<Room> room) {
    rooms.push_back(move(room));
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