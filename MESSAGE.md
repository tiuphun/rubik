# Message Documentation
*(Version 2.0) by Tieu Phuong*

This document provides details of all messages supported in the `MessageHandler` class. Each message type is detailed with their request (coming in) and response (returned by the handle functions).

## General Structure

### Request
The outermost field of a request is:
- `type`: The type of the message.
- `data`: The data part of the message.

### Response
The outermost field of a response is:
- `status`: The status of the response (e.g., "success" or "error").
- `data`: The data part of the response.

## Message Types

### 1. SIGN_UP

**Request:**
```json
{
    "type": "SIGN_UP",
    "data": {
        "username": "string",
        "password": "string"
    }
}
```

**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "string"
    }
}
```
### 2. SIGN_IN
**Request:**
```json
{
    "type": "SIGN_IN",
    "data": {
        "username": "string",
        "password": "string"
    }
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "You are signed in",
        "user_type": "ADMIN" | "PLAYER",
        "player_id": "int", // if user_type is PLAYER
        "admin_id": "int" // if user_type is ADMIN
    }
}
```

### 3. VIEW_ROOMS
**Request:**
```json
{
    "type": "VIEW_ROOMS",
    "data": {}
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Rooms fetched successfully",
        "rooms": [
            {
                "id": "int",
                "created_by": "int",
                "created_at": "time_t",
                "max_players": "int",
                "max_spectators": "int",
                "current_players": "int",
                "current_spectators": "int",
                "status": "RoomStatus"
            }
        ]
    }
}
```

### 4. CREATE_ROOM
**Request:**
```json
{
    "type": "CREATE_ROOM",
    "data": {
        "player_id": "int",
        "max_players": "int",
        "max_spectators": "int"
    }
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Room created successfully",
        "room_id": 1
    }
}
```

### 5. JOIN_ROOM
**Request:**
```json
{
    "type": "JOIN_ROOM",
    "data": {
        "player_id": "int",
        "room_id": "int",
        "participant_type": "PLAYER" | "PLAYER_SPECTATOR" | "ADMIN_SPECTATOR"
    }
}
```

**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Joined room successfully",'
        "room_id": 1
    }
}
```

### 6. LEAVE_ROOM
**Request:**
```json
{
    "type": "LEAVE_ROOM",
    "data": {
        "room_id": "int",
        "participant_id": "int"
    }
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Left room successfully"
    }
}
```

### 7. START_GAME
**Request:**
```json
{
    "type": "START_GAME",
    "data": {
        "player_id": 2,
        "room_id": 1
    }
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Game session started successfully",
        "session": {
            "id": "int",
            "room_id": "int",
            "player_id": "int",
            "start_time": "time_t",
            "end_time": "time_t",
            "total_moves": "int",
            "status": "STARTED" | "ENDED",
            "initial_cube_state": "string",
        }
    }
}
```

### 8. END_GAME
**Request:**
```json
{
    "type": "END_GAME",
    "data": {
        "player_id": 2,
        "game_session_id": 1
    }
}
```

**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Game session ended",
        "session": {
            "id": "int",
            "room_id": "int",
            "player_id": "int",
            "start_time": "time_t",
            "end_time": "time_t",
            "total_moves": "int",
            "status": "GameSessionStatus",
            "initial_cube_state": "string"
        }
    }
}
```

### 9. VIEW_USERS
**Request:**
```json
{
    "type": "VIEW_USERS",
    "data": {}
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Player list retrieved successfully",
        "players": [
            {
                "id": "int",
                "username": "string",
                "join_date": "time_t",
                "total_games": "int",
                "wins": "int",
                "best_time": "float",
                "avg_time": "float",
                "status": "PlayerStatus",
                "ban_date": "time_t",
                "ban_by": "int"
            }
        ]
    }
}
```

### 10. BAN_PLAYER
**Request:**
```json
{
    "type": "BAN_PLAYER",
    "data": {
        "admin_id": 4,
        "player_id": 3
    }
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Player banned successfully"
    }
}
```

### 11. SPECTATE
**Request:**
```json
{
    "type": "SPECTATE",
    "data": {
        "room_id": 2,
        "participant_id": 3
    }
}
```
**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Spectating started successfully"
    }
}
```

### 12. UPDATE_CUBE
**Request:**
```json
{
    "type": "UPDATE_CUBE",
    "data": {
        "player_id": "int",
        "game_session_id": "int",
        "new_cube_state": "string"
    }
}
```

**Response:**
```json
{
    "status": "success",
    "data": {
        "message": "Cube state updated successfully"
    }
}
```