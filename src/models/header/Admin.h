#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <ctime>

using namespace std;

class Admin {
public:
    int id;
    string username;
    string password_hash;
    time_t created_at;
    time_t last_login; 
    
    void banPlayer(int player_id) {
        // UPDATE player where player_id ...
        // get response...
    };
    void viewPlayerList() {
        // SELECT * from PLAYER...
    };

    void viewRoomList() {
        // SELECT room_id from Room WHERE status = 
    }

    void spectate(int game_session_id, int room_id){
        // status cua room id = 'Waiting'
        // Admin -> RoomParticipant voi vai tro la ADMIN_SPECTATOR

        // status cua room id = 'IN_PROGRESS'
        // polling lay cube cua moi nguoi.
    }
    
    void leaveGame() {
        // cook khoi phong
    }
};

#endif