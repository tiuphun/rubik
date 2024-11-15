// room_manager.h
#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <vector>
#include <nlohmann/json.hpp>

struct Room {
    int roomId;
    std::string createdBy;
    std::string status;
};

Room createRoom(const std::string& creator);
bool joinRoom(int roomId, const std::string& player);
std::vector<Room> getActiveRooms();

#endif
