#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "Query.h"

// PLAYER Queries:
const char* Query::CREATE_PLAYER_TABLE = 
    "CREATE TABLE IF NOT EXISTS Player ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "username TEXT UNIQUE NOT NULL,"
    "password_hash TEXT NOT NULL,"
    "join_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
    "total_games INTEGER DEFAULT 0,"
    "wins INTEGER DEFAULT 0,"
    "best_time REAL DEFAULT 0,"
    "avg_time REAL DEFAULT 0,"
    "status TEXT DEFAULT 'INACTIVE' CHECK (status IN ('ACTIVE', 'INACTIVE', 'BANNED')),"
    "ban_date DATETIME,"
    "ban_by INTEGER"
    "FOREIGN KEY (ban_by) REFERENCES Admin(id));"
    ;

const char* Query::INSERT_PLAYER = 
    "INSERT INTO Player(username,password_hash) "
    "VALUES(?,?);";

const char* Query::SELECT_ALL_PLAYER =
    "SELECT * FROM Player;";

// Admin Queries 

const char* Query::CREATE_ADMIN_TABLE =
    "CREATE TABLE IF NOT EXISTS Admin ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "username TEXT UNIQUE NOT NULL,"
    "password_hash TEXT NOT NULL,"
    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
    "last_login DATETIME);";

const char* Query::INSERT_ADMIN = 
    "INSERT INTO Admin(username,password_hash) "
    "VALUES(?,?);";

const char* Query::SELECT_ALL_ADMIN =
    "SELECT * FROM Admin";

//Room Queries

const char* Query::CREATE_ROOM_TABLE = 
    "CREATE TABLE IF NOT EXISTS Room ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "created_by INTEGER NOT NULL,"
    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
    "max_players INTEGER NOT NULL CHECK(max_players != 0),"
  	"max_spectators INTEGER NOT NULL,"
  	"FOREIGN KEY (created_by) REFERENCES Player(id) ON DELETE CASCADE);";

const char* Query::INSERT_ROOM = 
    "INSERT INTO Room(created_by,max_players,max_spectators)"
    "VALUES(?,?,?);";

const char* Query::SELECT_ALL_ROOM =
    "SELECT * FROM ROOM";

const char* Query::SELECT_ROOM_BY_ID =
    "SELECT * FROM ROOM"
    "WHERE id = ?;";

const char* Query::CREATE_ROOM_PARTICIPANT_TABLE = 
    "CREATE TABLE IF NOT EXISTS RoomParticipant ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "room_id INTEGER NOT NULL,"
    "player_id INTEGER NOT NULL,"
    "is_spectator INTEGER NOT NULL CHECK(is_spectator IN(0,1))"
  	"FOREIGN KEY (room_id) REFERENCES Room(id) ON DELETE CASCADE,"
    "FOREIGN KEY (player_id) REFERENCES Player(id) ON DELETE CASCADE);";

const char* Query::INSERT_ROOM_PARTICIPANT =
    "INSERT INTO RoomParticipant(room_id, player_id,is_spectator)"
    "VALUES (?,?,?)";

const char* Query::SELECT_ALL_ROOM_PLAYERS_BY_ROOM_ID =
    "SELECT * FROM RoomParticipant"
    "WHERE room_id = ? AND is_spectator = 0";

const char* Query::SELECT_ALL_ROOM_SPECTATORS_BY_ROOM_ID =
     "SELECT * FROM RoomParticipant"
    "WHERE room_id = ? AND is_spectator = 1";

const char* Query::CREATE_GAME_SESSION_TABLE = 
    "CREATE TABLE IF NOT EXISTS GameSession ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "room_id INTEGER NOT NULL, "
    "start_time DATETIME DEFAULT CURRENT_TIMESTAMP, "
    "end_time DATETIME, "
    "total_moves INTEGER DEFAULT 0, "
    "status TEXT DEFAULT 'in_progress' CHECK (status IN ('in_progress','completed')), "
    "initial_cube_state TEXT NOT NULL, "
    "winner_id INTEGER, "
    "FOREIGN KEY (winner_id) REFERENCES Player(id) ON DELETE CASCADE, "
    "FOREIGN KEY (room_id) REFERENCES Room(id) ON DELETE CASCADE"
    ");";

const char* Query::INSERT_GAME_SESSION =
    "INSERT INTO GameSession(room_id,initial_cube_state)"
    "VALUES (?,?)";

const char* Query::SELECT_ALL_GAME_SESSION =
    "SELECT * FROM GameSession;";

const char* Query::CREATE_PLAYER_GAME_SESSION_TABLE = 
    "CREATE TABLE IF NOT EXISTS PlayerGameSession ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "game_session_id INTEGER NOT NULL, "
    "player_id INTEGER NOT NULL, "
    "time_taken REAL, "
    "moves_count INTEGER, "
    "move_sequence TEXT, "
    "total_moves INTEGER DEFAULT 0, "
    "status TEXT DEFAULT 'playing' CHECK (status IN ('playing', 'abandoned', 'finished')), "
    "completion_time DATETIME, "
    "cube_state_id INTEGER NOT NULL, "
    "FOREIGN KEY (game_session_id) REFERENCES GameSession(id) ON DELETE CASCADE, "
    "FOREIGN KEY (player_id) REFERENCES Player(id) ON DELETE CASCADE, "
    "FOREIGN KEY (cube_state_id) REFERENCES CubeState(id) ON DELETE CASCADE"
    ");";

const char* Query::CREATE_GAME_MOVE_TABLE =
    "CREATE TABLE IF NOT EXISTS GameMove ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "player_game_session_id INTEGER NOT NULL, "
    "move TEXT NOT NULL, "
    "move_order INTEGER NOT NULL, "
    "move_timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
    "FOREIGN KEY (player_game_session_id) REFERENCES PlayerGameSession(id) ON DELETE CASCADE,"
    ");";

const char* Query::CREATE_CUBE_STATE_TABLE =
    "CREATE TABLE IF NOT EXISTS CubeState ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "player_game_session_id INTEGER NOT NULL, "
    "validated INTEGER DEFAULT 1 CHECK(validated IN (1, 0)), "
    "cube TEXT, "
    "validation_timestamp DATETIME, "
    "FOREIGN KEY (player_game_session_id) REFERENCES PlayerGameSession(id),"
    ");";








