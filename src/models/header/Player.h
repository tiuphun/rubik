// include/Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <string.h>
#include "Const.h"

using namespace std;

class Player {
public:
    int id;
    string username;
    string password_hash;
    time_t join_date;
    int total_games;
    int wins;
    float best_time;
    float avg_time;
    PlayerStatus status;
    time_t ban_date;
    int ban_by;

    
    int socket_fd;
    //bool is_logged_in;

    /*
    Player(string uname, int sock)
        : username(uname), socket_fd(sock), is_logged_in(true) {}

    void logout() {
        is_logged_in = false;
    }
    */
    int createRoom(int created_by, int max_players, int max_spectators);
    void joinRoom(Player player_id);
    
    void readyRequestResponse(int player_id) {
        // update is_ready trong bang Room Participant voi player_id tuong ung 
    };

    
};

#endif
