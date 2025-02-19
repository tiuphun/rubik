#ifndef CONST_H
#define CONST_H

enum PlayerStatus{
   ACTIVE,
   INACTIVE,
   BANNED
};

enum RoomStatus{
    WAITING,
    READY,
    IN_PROGRESS,
    ENDED
};

enum RoomParticipantStatus{
    PLAYER,
    PLAYER_SPECTATOR,
    ADMIN_SPECTATOR
};

enum GameSessionStatus{
    IN_SESSION,
    COMPLETED
};

enum PlayerGameSessionStatus{
    PLAYING,
    ABANDONED,
    FINISHED
};

enum ValidationStatus{
    SUCCESS,
    FAIL
};


inline RoomParticipantStatus stringToRoomParticipantStatus(const std::string& status) {
    if (status == "PLAYER") {
        return RoomParticipantStatus::PLAYER;
    } else if (status == "PLAYER_SPECTATOR") {
        return RoomParticipantStatus::PLAYER_SPECTATOR;
    } else if(status == "ADMIN_SPECTATOR"){
        return RoomParticipantStatus::ADMIN_SPECTATOR;
    }else {
        throw std::invalid_argument("Invalid RoomParticipantStatus value");
    }
}



#endif 