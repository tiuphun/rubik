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
        "player_id": 5, // if user_type is PLAYER
        "admin_id": 1 // if user_type is ADMIN
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
                "id": 2,
                "created_by": 1,
                "created_at": "time_t",
                "max_players": 2,
                "max_spectators": 2,
                "current_players": 2,
                "current_spectators": 2,
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
        "player_id": 2,
        "max_players": 2,
        "max_spectators": 2
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
        "player_id": 2,
        "room_id": 2,
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
        "room_id": 2,
        "participant_id": 2
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
            "id": 2,
            "room_id": 2,
            "player_id": 2,
            "start_time": "time_t",
            "end_time": "time_t",
            "total_moves": 2,
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
            "id": 1,
            "room_id": 2,
            "player_id": 3,
            "start_time": "time_t",
            "end_time": "time_t",
            "total_moves": 4,
            "status": "ENDED",
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
                "id": 1,
                "username": "string",
                "join_date": "time_t",
                "total_games": 5,
                "wins": 3,
                "best_time": "float",
                "avg_time": "float",
                "status": "ACTIVE" | "BANNED" | "INACTIVE",
                "ban_date": "time_t",
                "ban_by": 5 //admin_id
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
        "player_id": 2,
        "game_session_id": 2,
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