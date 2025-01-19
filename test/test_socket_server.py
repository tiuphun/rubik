import socket
import json

def handle_client(client_socket):
    request = client_socket.recv(1024).decode('utf-8')
    
    if not request:
        print("Received an empty request")
        client_socket.close()
        return
    
    print(f"Received: {request}")
    
    try:
        data = json.loads(request)
    except json.JSONDecodeError as e:
        print(f"JSON decode error: {e}")
        client_socket.close()
        return
    
    request_type = data.get('type')
    username = data['data'].get('username') if data.get('data') else None
    password = data['data'].get('password') if data.get('data') else None
    room_id = data['data'].get('room_id') if data.get('data') else None
    participant_type = data['data'].get('participant_type') if data.get('data') else None
    max_players = data['data'].get('max_players') if data.get('data') else None
    max_spectators = data['data'].get('max_spectators') if data.get('data') else None
    player_game_session_id = data['data'].get('player_game_session_id') if data.get('data') else None
    current_cube = data['data'].get('current_cube') if data.get('data') else None

    # Determine the response based on the request type
    if request_type == "SIGN_IN":
        if username == "admin":
            response = json.dumps({
                "status": "success",
                "data": {
                    "message": "User signed in successfully",
                    "user_type": "ADMIN",
                    "player": {
                        "id": 1,
                        "username": "admin",
                        "password_hash": "hashed_password",
                        "join_date": "2023-10-01T12:34:56Z",
                        "total_games": 100,
                        "wins": 50,
                        "best_time": 30.5,
                        "avg_time": 45.2,
                        "status": "active",
                        "ban_date": None,
                        "ban_by": None,
                        "socket_fd": 5
                    }
                }
            })
        elif username == "user":
            response = json.dumps({
                "status": "success",
                "data": {
                    "message": "User signed in successfully",
                    "user_type": "PLAYER",
                    "player": {
                        "id": 2,
                        "username": "user",
                        "password_hash": "hashed_password",
                        "join_date": "2023-10-01T12:34:56Z",
                        "total_games": 100,
                        "wins": 50,
                        "best_time": 30.5,
                        "avg_time": 45.2,
                        "status": "active",
                        "ban_date": None,
                        "ban_by": None,
                        "socket_fd": 5
                    }
                }
            })
        else:
            response = json.dumps({
                "status": "fail",
                "data": {
                    "message": "Invalid credentials"
                }
            })
    elif request_type == "SIGN_UP":
        # For simplicity, assume all sign-up attempts are successful
        response = json.dumps({
            "status": "success",
            "data": {
                "message": "User signed up successfully",
                "user_type": "PLAYER"
            }
        })
    elif request_type == "VIEW_ROOMS":
        response = json.dumps({
            "status": "success",
            "data": {
                "rooms": [
                    {
                        "id": 1,
                        "max_players": 4,
                        "max_spectators": 10,
                        "status": "WAITING",
                        "current_players": 2,
                        "current_spectators": 3,
                    },
                    {
                        "id": 2,
                        "max_players": 6,
                        "max_spectators": 12,
                        "status": "IN_PROGRESS",
                        "current_players": 5,
                        "current_spectators": 7,
                    }
                ]
            }
        })
    elif request_type == "JOIN_ROOM":
        # Respond with room participants
        response = json.dumps({
            "status": "success",
            "data": {
                "message": "Joined room successfully",
                "room_participants": [
                    {
                        "id": 1,
                        "room_id": room_id,
                        "participant_type": "PLAYER",
                        "participant_id": 456,
                        "is_ready": False,
                    },
                    {
                        "id": 2,
                        "room_id": room_id,
                        "participant_type": "PLAYER_SPECTATOR",
                        "participant_id": 457,
                        "is_ready": True,
                    },
                    {
                        "id": 3,
                        "room_id": room_id,
                        "participant_type": "ADMIN_SPECTATOR",
                        "participant_id": 458,
                        "is_ready": False,
                    }
                ]
            }
        })
    elif request_type == "CREATE_ROOM":
        # Respond with room creation confirmation
        response = json.dumps({
            "status": "success",
            "data": {
                "message": "Room created successfully",
                "room": {
                    "room_id": 123,
                    "max_players": max_players,
                    "max_spectators": max_spectators
                }
            }
        })
    elif request_type == "READY":
        response = json.dumps({
            "status": "success",
            "data": {
                "message": "User is ready"
            }
        })
    elif request_type == "START_GAME":
        response = json.dumps({
            "status": "success",
            "data": {
                "game_session": {
                    "id": 1,
                    "room_id": 123,
                    "player_id": 1,
                    "start_time": "2023-10-01T12:34:56Z",
                    "total_moves": 0,
                    "status": "IN_SESSION",
                    "initial_cube_state": "Y O Y Y Y O Y B B W B G B W B Y R Y R W R W R R G B G O G Y G G O G O R O R O B W O W G W B W R",
                }
            }
        })
    elif request_type == "END_GAME":
        response = json.dumps({
            "status": "success",
            "data": {
                "message": "Game ended successfully"
            }
        })
    elif request_type == "LEAVE_ROOM":
        response = json.dumps({
            "status": "success",
            "data": {
                "message": "Left room successfully"
            }
        })
    elif request_type == "VIEW_USERS":
        response = json.dumps({
            "status": "success",
            "data": {
                "users": [
                    {
                        "id": 1,
                        "username": "player1",
                        "password_hash": "hashed_password1",
                        "total_games": 100,
                        "wins": 50,
                        "best_time": 30.5,
                        "avg_time": 45.2,
                        "status": "active",
                        "ban_by": None,
                        "socket_fd": 5
                    },
                    {
                        "id": 2,
                        "username": "player2",
                        "password_hash": "hashed_password2",
                        "total_games": 80,
                        "wins": 40,
                        "best_time": 28.7,
                        "avg_time": 42.1,
                        "status": "banned",
                        "ban_by": 1,
                        "socket_fd": 6
                    }
                ]
            }
        })
    else:
        response = json.dumps({
            "status": "fail",
            "data": {
                "message": "Unknown request type"
            }
        })

    client_socket.send(response.encode())
    client_socket.close()

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("0.0.0.0", 5000))
    server.listen(5)
    print("Server listening on port 5000")

    while True:
        client_socket, addr = server.accept()
        print(f"Accepted connection from {addr}")
        handle_client(client_socket)

if __name__ == "__main__":
    main()