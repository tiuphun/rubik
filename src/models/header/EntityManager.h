#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "Player.h"
#include "Admin.h"
#include "Room.h"
#include "RoomParticipant.h"
#include "GameSession.h"
#include <map>

using namespace std;
class EntityManager {
public:
    static EntityManager& getInstance() {
        static EntityManager instance;
        return instance;
    }
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    
    void addPlayer(unique_ptr<Player> player);
    void addAdmin(unique_ptr<Admin> admin);
    void addRoom(unique_ptr<Room> room);
    void addRoomParticipant(unique_ptr<RoomParticipant> rp);
    void addGameSession(unique_ptr<GameSession> gs);

    Player* getPlayerById(int id);
    Admin* getAdminById(int id);
    Room* getRoomById(int id);
    RoomParticipant * getRoomParticipantById(int id);
    GameSession *getGameSessionbyId(int id);

    const vector<unique_ptr<Player>>& getAllPlayers() const;
    const vector<unique_ptr<Admin>>& getAllAdmins() const;
    const vector<unique_ptr<Room>>& getAllRooms() const;
    const vector<unique_ptr<GameSession>>& getAllGameSession() const;
    const vector<unique_ptr<RoomParticipant>>& getAllRoomParticipant() const;
    

    int getNextRoomId() { return roomIdCounter++; }
    int getNextGameSessionId() { return gameSessionIdCounter++; }

    void removePlayer(int id);
    void removeAdmin(int id);
    void removeRoom(int id);
    void removeRoomParticipant(int id);
    void removeGameSession(int id);
    vector<RoomParticipant*> getRoomParticipants(int roomId) const;
    vector<GameSession*> getRoomGameSessions(int roomId) const;

private:
    EntityManager() = default;
    vector<unique_ptr<Player>> players;
    vector<unique_ptr<Admin>> admins;
    vector<unique_ptr<Room>> rooms;
    vector<unique_ptr<RoomParticipant>> participants;
    vector<unique_ptr<GameSession>> gameSessions;

    std::map<int, std::vector<int>> roomToParticipants;    // room_id -> participant_ids
    std::map<int, std::vector<int>> roomToGameSessions;    // room_id -> game_session_ids
    static int roomIdCounter;
    static int gameSessionIdCounter;
};

#endif