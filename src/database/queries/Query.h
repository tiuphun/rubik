#ifndef QUERY_H
#define QUERY_H

class Query {
public:
    // PRAGMA to enable foreign keys
    static const char* ENABLE_FOREIGN_KEYS;

    // Admin Queries
    static const char* CREATE_ADMIN_TABLE;
    static const char* INSERT_ADMIN;
    static const char* SELECT_ALL_ADMIN;
    static const char* BAN_PLAYER;

    // Player Queries
    static const char* CREATE_PLAYER_TABLE;
    static const char* INSERT_PLAYER;
    static const char* SELECT_ALL_PLAYER;
    static const char* SELECT_PLAYER_BY_USERNAME;

    // Room Queries
    static const char* CREATE_ROOM_TABLE;
    static const char* INSERT_ROOM;
    static const char* SELECT_ALL_ROOM;
    static const char* SELECT_ROOM_BY_ID;
    static const char* SELECT_ALL_AVAILABLE_ROOM;
    static const char* CHECK_ROOM_STATUS;

    // RoomParticipant Queries
    static const char* CREATE_ROOM_PARTICIPANT_TABLE;
    static const char* INSERT_ROOM_PARTICIPANT;
    static const char* SELECT_ALL_ROOM_PLAYERS_BY_ROOM_ID;
    static const char* SELECT_ALL_ROOM_SPECTATORS_BY_ROOM_ID;
    static const char* DELETE_FROM_ROOM_PARTICIPANT;
    static const char* UPDATE_ROOM_PARTICIPANT_READY;

    // GameSession Queries
    static const char* CREATE_GAME_SESSION_TABLE;
    static const char* INSERT_GAME_SESSION;
    static const char* SELECT_ALL_GAME_SESSION;

    // PlayerGameSession Queries
    static const char* CREATE_PLAYER_GAME_SESSION_TABLE;

    // CubeState Queries
    static const char* CREATE_CUBE_STATE_TABLE;
    static const char* INSERT_CUBE_STATE;

    // GameMove Queries
    static const char* CREATE_GAME_MOVE_TABLE;
    static const char* INSERT_GAME_MOVE;

    // View and Index Definitions
    static const char* CREATE_ROOM_PLAYERS_VIEW;
    static const char* CREATE_ROOM_SPECTATORS_VIEW;
    static const char* CREATE_ROOM_PARTICIPANT_COUNTS_VIEW;

    static const char* CREATE_PLAYER_USERNAME_INDEX;
    static const char* CREATE_ADMIN_USERNAME_INDEX;
    static const char* CREATE_ROOM_STATUS_INDEX;
    static const char* CREATE_ROOM_PARTICIPANT_TYPES_INDEX;
    static const char* CREATE_GAME_SESSION_STATUS_INDEX;
    static const char* CREATE_PLAYER_GAME_SESSION_STATUS_INDEX;
    static const char* CREATE_GAME_MOVES_SESSION_INDEX;
};

#endif 
