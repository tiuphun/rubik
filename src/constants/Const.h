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
    COMPLETED
};

enum GameSessionStatus{
    IN_PROGRESS,
    COMPLETED
};

enum PlayerGameSessionStatus{
    PLAYING,
    ABANDONED
};

enum ValidationStatus{
    success,
    fail
};

#endif 