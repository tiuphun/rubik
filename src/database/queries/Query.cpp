#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Query.h"

/* DATABASE SCHEMA:
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS Admin (
    id INTEGER PRIMARY KEY,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    created_at DATETIME DEFAULT (datetime('now')),
    last_login DATETIME
);

CREATE TABLE IF NOT EXISTS Player (
    id INTEGER PRIMARY KEY,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    join_date DATETIME DEFAULT (datetime('now')),
    total_games INTEGER DEFAULT 0,
    wins INTEGER DEFAULT 0,
    best_time REAL DEFAULT 0,
    avg_time REAL DEFAULT 0,
    status TEXT DEFAULT 'ACTIVE' CHECK (status IN ('ACTIVE', 'INACTIVE', 'BANNED')),
    ban_date DATETIME,
    ban_by INTEGER,
    FOREIGN KEY (ban_by) REFERENCES Admin(id)
);

CREATE TABLE IF NOT EXISTS Room (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created_by INTEGER NOT NULL,
    created_at DATETIME DEFAULT (datetime('now')),
    max_players INTEGER NOT NULL CHECK(max_players > 0),
    max_spectators INTEGER NOT NULL CHECK(max_spectators >= 0),
    status TEXT DEFAULT 'WAITING' CHECK (status IN ('WAITING', 'READY', 'IN_PROGRESS', 'ENDED')),
    FOREIGN KEY (created_by) REFERENCES Player(id) ON DELETE RESTRICT
);

CREATE TABLE IF NOT EXISTS RoomParticipant (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    room_id INTEGER NOT NULL,
    participant_type TEXT NOT NULL CHECK(participant_type IN ('PLAYER', 'PLAYER_SPECTATOR', 'ADMIN_SPECTATOR')),
    participant_id INTEGER NOT NULL,
    is_ready INTEGER DEFAULT 0 CHECK(is_ready IN (0,1)),
    joined_at DATETIME DEFAULT (datetime('now')),
    UNIQUE(room_id, participant_id, participant_type),
    FOREIGN KEY (room_id) REFERENCES Room(id) ON DELETE RESTRICT
);

CREATE TABLE IF NOT EXISTS GameSession (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    room_id INTEGER NOT NULL,
    start_time DATETIME DEFAULT (datetime('now')),
    end_time DATETIME,
    total_moves INTEGER DEFAULT 0,
    status TEXT DEFAULT 'IN_SESSION' CHECK (status IN ('IN_SESSION', 'COMPLETED')),
    initial_cube_state INTEGER NOT NULL,
    winner_id INTEGER,
    FOREIGN KEY (room_id) REFERENCES Room(id) ON DELETE RESTRICT,
    FOREIGN KEY (winner_id) REFERENCES Player(id),
  	FOREIGN KEY (initial_cube_state) REFERENCES CubeState(id)
);

CREATE TABLE IF NOT EXISTS PlayerGameSession (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    game_session_id INTEGER NOT NULL,
    player_id INTEGER NOT NULL,
    time_taken REAL,
    moves_count INTEGER DEFAULT 0,
    status TEXT DEFAULT 'PLAYING' CHECK (status IN ('PLAYING', 'ABANDONED', 'FINISHED')),
    completion_time DATETIME,
    cube_state_id INTEGER,
    FOREIGN KEY (game_session_id) REFERENCES GameSession(id) ON DELETE RESTRICT,
    FOREIGN KEY (player_id) REFERENCES Player(id),
    FOREIGN KEY (cube_state_id) REFERENCES CubeState(id)
);

CREATE TABLE IF NOT EXISTS CubeState (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_game_session_id INTEGER NOT NULL,
    status TEXT DEFAULT 'SUCCESS' CHECK (status IN('SUCCESS','FAIL')),
    cube TEXT NOT NULL,
    validation_timestamp DATETIME DEFAULT (datetime('now')),
    FOREIGN KEY (player_game_session_id) REFERENCES PlayerGameSession(id) ON DELETE RESTRICT
);

CREATE VIEW IF NOT EXISTS RoomPlayers AS
SELECT 
    rp.*, 
    p.username
FROM RoomParticipant rp
JOIN Player p ON rp.participant_id = p.id
WHERE rp.participant_type = 'PLAYER';

CREATE VIEW IF NOT EXISTS RoomSpectators AS
SELECT 
    rp.*,
    CASE 
        WHEN rp.participant_type = 'ADMIN_SPECTATOR' THEN a.username
        ELSE p.username
    END as username
FROM RoomParticipant rp
LEFT JOIN Player p ON rp.participant_id = p.id AND rp.participant_type = 'PLAYER_SPECTATOR'
LEFT JOIN Admin a ON rp.participant_id = a.id AND rp.participant_type = 'ADMIN_SPECTATOR'
WHERE rp.participant_type IN ('PLAYER_SPECTATOR', 'ADMIN_SPECTATOR');

CREATE VIEW IF NOT EXISTS RoomParticipantCounts AS
SELECT 
    room_id,
    SUM(CASE WHEN participant_type = 'PLAYER' THEN 1 ELSE 0 END) as player_count,
    SUM(CASE WHEN participant_type IN ('PLAYER_SPECTATOR', 'ADMIN_SPECTATOR') THEN 1 ELSE 0 END) as spectator_count
FROM RoomParticipant
GROUP BY room_id;

CREATE VIEW IF NOT EXISTS AuthUsers AS
SELECT 
    id,
    username,
    password_hash,
    'PLAYER' as user_type,
    status as account_status
FROM Player
UNION ALL
SELECT 
    id,
    username,
    password_hash,
    'ADMIN' as user_type,
    'ACTIVE' as account_status  -- Admins are always considered active
FROM Admin;

CREATE INDEX IF NOT EXISTS idx_player_username ON Player(username);
CREATE INDEX IF NOT EXISTS idx_admin_username ON Admin(username);
CREATE INDEX IF NOT EXISTS idx_room_status ON Room(status);
CREATE INDEX IF NOT EXISTS idx_room_participant_types ON RoomParticipant(room_id, participant_type);
CREATE INDEX IF NOT EXISTS idx_game_session_status ON GameSession(status);
CREATE INDEX IF NOT EXISTS idx_player_game_session_status ON PlayerGameSession(status);
CREATE INDEX IF NOT EXISTS idx_cube_state_validation ON CubeState(validation_timestamp);
CREATE INDEX IF NOT EXISTS idx_game_session_times ON GameSession(start_time, end_time);
CREATE INDEX IF NOT EXISTS idx_cube_state_id ON PlayerGameSession(cube_state_id);
*/

// PRAGMA to enable foreign keys
const char* Query::ENABLE_FOREIGN_KEYS = "PRAGMA foreign_keys = ON;";

// Admin Queries
const char* Query::CREATE_ADMIN_TABLE = 
    "CREATE TABLE IF NOT EXISTS Admin ("
    "id INTEGER PRIMARY KEY,"
    "username TEXT UNIQUE NOT NULL,"
    "password_hash TEXT NOT NULL,"
    "created_at DATETIME DEFAULT (datetime('now')),"
    "last_login DATETIME"
    ");";
const char* Query::FIND_AUTH_USER = 
    "SELECT id, username, user_type, account_status FROM AuthUsers WHERE username = ? AND password_hash = ? LIMIT 1;";

const char* Query::INSERT_ADMIN = 
    "INSERT INTO Admin(id,username,password_hash) "
    "VALUES(?,?,?);";

const char* Query::SELECT_ALL_ADMIN =
    "SELECT * FROM Admin;";

const char* Query::SELECT_ADMIN_BY_ID =
    "SELECT * FROM admin WHERE id = ? LIMIT 1;";

const char* Query::BAN_PLAYER =
    "UPDATE Player SET status = 'BANNED', ban_by = ? WHERE id = ?;";
    
// Player Queries
const char* Query::CREATE_PLAYER_TABLE = 
    "CREATE TABLE IF NOT EXISTS Player ("
    "id INTEGER PRIMARY KEY,"
    "username TEXT UNIQUE NOT NULL,"
    "password_hash TEXT NOT NULL,"
    "join_date DATETIME DEFAULT (datetime('now')),"
    "total_games INTEGER DEFAULT 0,"
    "wins INTEGER DEFAULT 0,"
    "best_time REAL DEFAULT 0,"
    "avg_time REAL DEFAULT 0,"
    "status TEXT DEFAULT 'ACTIVE' CHECK (status IN ('ACTIVE', 'INACTIVE', 'BANNED')),"
    "ban_date DATETIME,"
    "ban_by INTEGER,"
    "FOREIGN KEY (ban_by) REFERENCES Admin(id)"
    ");";

const char* Query::INSERT_PLAYER = 
    "INSERT INTO Player(username,password_hash) "
    "VALUES(?,?);";

const char* Query::SELECT_ALL_PLAYER =
    "SELECT * FROM Player;";

const char* Query::SELECT_PLAYER_BY_USERNAME =
    "SELECT * FROM Player WHERE username = ?;";

const char* Query::SELECT_PLAYER_BY_ID =
    "SELECT * FROM Player WHERE id = ?;";

// Room Queries
const char* Query::CREATE_ROOM_TABLE = 
    "CREATE TABLE IF NOT EXISTS Room ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "created_by INTEGER NOT NULL,"
    "created_at DATETIME DEFAULT (datetime('now')),"
    "max_players INTEGER NOT NULL CHECK(max_players > 0),"
    "max_spectators INTEGER NOT NULL CHECK(max_spectators >= 0),"
    "status TEXT DEFAULT 'WAITING' CHECK (status IN ('WAITING', 'READY', 'IN_PROGRESS', 'ENDED')),"
    "FOREIGN KEY (created_by) REFERENCES Player(id) ON DELETE CASCADE"
    ");";

const char* Query::INSERT_ROOM = 
    "INSERT INTO Room(created_by,max_players,max_spectators)"
    "VALUES(?,?,?);";

const char* Query::SELECT_ALL_ROOM =
    "SELECT * FROM Room;";

const char* Query::SELECT_ROOM_BY_ID =
    "SELECT * FROM Room WHERE id = ?;";

const char* Query::SELECT_ALL_AVAILABLE_ROOM =
    "SELECT * FROM Room WHERE status = 'WAITING';";

const char* Query::CHECK_ROOM_STATUS =
    "SELECT status FROM Room WHERE id = ?;";

// RoomParticipant Queries
const char* Query::CREATE_ROOM_PARTICIPANT_TABLE = 
    "CREATE TABLE IF NOT EXISTS RoomParticipant ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "room_id INTEGER NOT NULL,"
    "participant_type TEXT NOT NULL CHECK(participant_type IN ('PLAYER', 'PLAYER_SPECTATOR', 'ADMIN_SPECTATOR')),"
    "participant_id INTEGER NOT NULL,"
    "is_ready INTEGER DEFAULT 0 CHECK(is_ready IN (0,1)),"
    "joined_at DATETIME DEFAULT (datetime('now')),"
    "UNIQUE(room_id, participant_id, participant_type),"
    "FOREIGN KEY (room_id) REFERENCES Room(id) ON DELETE CASCADE,"
    "FOREIGN KEY (participant_id) REFERENCES Player(id)"
    ");";

const char* Query::INSERT_ROOM_PARTICIPANT =
    "INSERT INTO RoomParticipant(room_id, participant_type, participant_id, is_ready) "
    "VALUES(?,?,?,?);";

const char* Query::SELECT_ALL_ROOM_PLAYERS_BY_ROOM_ID =
    "SELECT * FROM RoomParticipant WHERE room_id = ? AND participant_type = 'PLAYER';";

const char* Query::SELECT_ALL_ROOM_SPECTATORS_BY_ROOM_ID =
    "SELECT * FROM RoomParticipant WHERE room_id = ? AND participant_type IN ('PLAYER_SPECTATOR', 'ADMIN_SPECTATOR');";

const char* Query::DELETE_FROM_ROOM_PARTICIPANT =
    "DELETE FROM RoomParticipant WHERE room_id = ? AND id = ?;";

const char* Query::UPDATE_ROOM_PARTICIPANT_READY =
    "UPDATE RoomParticipant SET is_ready = 1 WHERE room_id = ? AND participant_id = ? AND participant_type = 'PLAYER';";

// GameSession Queries
const char* Query::CREATE_GAME_SESSION_TABLE = 
    "CREATE TABLE IF NOT EXISTS GameSession ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "room_id INTEGER NOT NULL,"
    "start_time DATETIME DEFAULT (datetime('now')),"
    "end_time DATETIME,"
    "total_moves INTEGER DEFAULT 0,"
    "status TEXT DEFAULT 'IN_SESSION' CHECK (status IN ('IN_SESSION', 'COMPLETED')),"
    "initial_cube_state INTEGER NOT NULL,"
    "winner_id INTEGER,"
    "FOREIGN KEY (room_id) REFERENCES Room(id) ON DELETE CASCADE,"
    "FOREIGN KEY (winner_id) REFERENCES Player(id)"
    ");";

const char* Query::INSERT_GAME_SESSION =
    "INSERT INTO GameSession(room_id, initial_cube_state) "
    "VALUES(?,?);";

const char* Query::SELECT_ALL_GAME_SESSION =
    "SELECT * FROM GameSession;";

const char* Query::END_GAME_SESSION =
    "UPDATE GameSession SET status = 'COMPLETED', end_time = datetime('now') WHERE id = ?;";

// PlayerGameSession Queries
const char* Query::CREATE_PLAYER_GAME_SESSION_TABLE = 
    "CREATE TABLE IF NOT EXISTS PlayerGameSession ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "game_session_id INTEGER NOT NULL,"
    "player_id INTEGER NOT NULL,"
    "time_taken REAL,"
    "moves_count INTEGER DEFAULT 0,"
    "move_sequence TEXT,"
    "status TEXT DEFAULT 'PLAYING' CHECK (status IN ('PLAYING', 'ABANDONED', 'FINISHED')),"
    "completion_time DATETIME,"
    "cube_state_id INTEGER,"
    "FOREIGN KEY (game_session_id) REFERENCES GameSession(id) ON DELETE CASCADE,"
    "FOREIGN KEY (player_id) REFERENCES Player(id),"
    "FOREIGN KEY (cube_state_id) REFERENCES CubeState(id)"
    ");";

const char* Query::INSERT_PLAYER_GAME_SESSION =
    "INSERT INTO PlayerGameSession(game_session_id, player_id,cube_state_id) "
    "VALUES(?,?,?);";

// CubeState Queries
const char* Query::CREATE_CUBE_STATE_TABLE = 
    "CREATE TABLE IF NOT EXISTS CubeState ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "player_game_session_id INTEGER NOT NULL,"
    "status TEXT DEFAULT 'SUCCESS' CHECK (status IN('SUCCESS','FAIL')),"
    "cube TEXT NOT NULL,"
    "validation_timestamp DATETIME DEFAULT (datetime('now')),"
    "FOREIGN KEY (player_game_session_id) REFERENCES PlayerGameSession(id) ON DELETE CASCADE"
    ");";

const char* Query::INSERT_CUBE_STATE =
    "INSERT INTO CubeState(player_game_session_id, status, cube) "
    "VALUES(?,?,?);";

// View and Index Definitions
const char* Query::CREATE_ROOM_PLAYERS_VIEW =
    "CREATE VIEW IF NOT EXISTS RoomPlayers AS "
    "SELECT rp.*, p.username FROM RoomParticipant rp "
    "JOIN Player p ON rp.participant_id = p.id "
    "WHERE rp.participant_type = 'PLAYER';";

const char* Query::CREATE_ROOM_SPECTATORS_VIEW =
    "CREATE VIEW IF NOT EXISTS RoomSpectators AS "
    "SELECT rp.*, CASE WHEN rp.participant_type = 'ADMIN_SPECTATOR' THEN a.username ELSE p.username END as username "
    "FROM RoomParticipant rp "
    "LEFT JOIN Player p ON rp.participant_id = p.id AND rp.participant_type = 'PLAYER_SPECTATOR' "
    "LEFT JOIN Admin a ON rp.participant_id = a.id AND rp.participant_type = 'ADMIN_SPECTATOR' "
    "WHERE rp.participant_type IN ('PLAYER_SPECTATOR', 'ADMIN_SPECTATOR');";

const char* Query::CREATE_ROOM_PARTICIPANT_COUNTS_VIEW =
    "CREATE VIEW IF NOT EXISTS RoomParticipantCounts AS "
    "SELECT room_id, "
    "SUM(CASE WHEN participant_type = 'PLAYER' THEN 1 ELSE 0 END) as player_count, "
    "SUM(CASE WHEN participant_type IN ('PLAYER_SPECTATOR', 'ADMIN_SPECTATOR') THEN 1 ELSE 0 END) as spectator_count "
    "FROM RoomParticipant GROUP BY room_id;";

const char* Query::CREATE_PLAYER_USERNAME_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_player_username ON Player(username);";

const char* Query::CREATE_ADMIN_USERNAME_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_admin_username ON Admin(username);";

const char* Query::CREATE_ROOM_STATUS_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_room_status ON Room(status);";

const char* Query::CREATE_ROOM_PARTICIPANT_TYPES_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_room_participant_types ON RoomParticipant(room_id, participant_type);";

const char* Query::CREATE_GAME_SESSION_STATUS_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_game_session_status ON GameSession(status);";

const char* Query::CREATE_PLAYER_GAME_SESSION_STATUS_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_player_game_session_status ON PlayerGameSession(status);";

const char* Query::CREATE_GAME_MOVES_SESSION_INDEX =
    "CREATE INDEX IF NOT EXISTS idx_game_moves_session ON GameMove(player_game_session_id, move_timestamp);";
