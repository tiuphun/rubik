#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "../header/Player.h"
#include "../header/Admin.h"
#include "../header/Room.h"

using namespace std;
class EntityManager {
public:
    void addPlayer(unique_ptr<Player> player);
    void addAdmin(unique_ptr<Admin> admin);
    void addRoom(unique_ptr<Room> room);

    Player* getPlayerById(int id);
    Admin* getAdminById(int id);
    Room* getRoomById(int id);

    const vector<unique_ptr<Player>>& getAllPlayers() const;
    const vector<unique_ptr<Admin>>& getAllAdmins() const;
    const vector<unique_ptr<Room>>& getAllRooms() const;

    void removePlayer(int id);
    void removeAdmin(int id);
    void removeRoom(int id);

private:
    vector<unique_ptr<Player>> players;
    vector<unique_ptr<Admin>> admins;
    vector<unique_ptr<Room>> rooms;
};

#endif