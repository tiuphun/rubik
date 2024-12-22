#ifndef QUERY_H
#define QUERY_H

using namespace std;

class Query {
   static const char* CREATE_PLAYER_TABLE;

   static const char* INSERT_PLAYER;
   static const char* SELECT_ALL_PLAYER;
   
   static const char* CREATE_ADMIN_TABLE;
   static const char* INSERT_ADMIN;
   static const char* SELECT_ALL_ADMIN;

   static const char* CREATE_ROOM_TABLE;
   static const char* INSERT_ROOM;
   static const char* SELECT_ALL_ROOM;
   static const char* SELECT_ROOM_BY_ID;

   static const char* CREATE_ROOM_PARTICIPANT_TABLE;
   static const char* INSERT_ROOM_PARTICIPANT;
   static const char* SELECT_ALL_ROOM_PLAYERS_BY_ROOM_ID;
   static const char* SELECT_ALL_ROOM_SPECTATORS_BY_ROOM_ID;
    
   static const char* CREATE_GAME_SESSION_TABLE;
   static const char* INSERT_GAME_SESSION;
   static const char* SELECT_ALL_GAME_SESSION;

   static const char* CREATE_PLAYER_GAME_SESSION_TABLE;
   static const char* INSERT_PLAYER_GAME_SESSION;
   static const char* SELECT_ALL_PLAYER_GAME_SESSION;

   static const char* CREATE_GAME_MOVE_TABLE;
   static const char* INSERT_GAME_MOVE;
   static const char* SELECT_GAME_MOVE_BY_ID;

   static const char* CREATE_CUBE_STATE_TABLE;
   static const char* INSERT_CUBE_STATE;

   
};

#endif