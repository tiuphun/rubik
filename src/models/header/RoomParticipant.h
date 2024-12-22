#ifndef ROOMPARTICIPANT_H
#define ROOMPARTICIPANT_H

#include <string.h>
#include <stdio.h>
#include <ctime>

using namespace std;

class RoomParticipant {
    public:
        int id;
        int room_id;
        int player_id;
        bool is_spectator;
        bool is_ready;
        time_t joined_at;

    
};

#endif