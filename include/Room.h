// include/Room.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <algorithm>
#include "Player.h"

using namespace std;

class Room {
public:
    string room_name;
    vector<Player*> players;

    Room(string name) : room_name(name) {}

    void add_player(Player* player) {
        players.push_back(player);
    }

    void remove_player(Player* player) {
        players.erase(remove(players.begin(), players.end(), player), players.end());
    }
};

#endif
