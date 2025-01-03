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

#endif 