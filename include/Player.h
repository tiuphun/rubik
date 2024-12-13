// include/Player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;

class Player {
public:
    string username;
    int socket_fd;
    bool is_logged_in;

    Player(string uname, int sock)
        : username(uname), socket_fd(sock), is_logged_in(true) {}

    void logout() {
        is_logged_in = false;
    }
};

#endif
