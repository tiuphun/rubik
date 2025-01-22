import socket
import json

def send_request(request):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('127.0.0.1', 8080))
    client_socket.send(request.encode('utf-8'))
    
    response = client_socket.recv(4096).decode('utf-8')
    print(f"Response: {response}")
    
    client_socket.close()

if __name__ == "__main__":
    # Example request to view rooms
    view_rooms_request = json.dumps({
        "type": "VIEW_ROOMS",
        "data": None
    })
    
    send_request(view_rooms_request)
    
    # Example request to sign up
    sign_up_request = json.dumps({
        "type": "SIGN_UP",
        "data": {
            "username": "user",
            "password": "user"
        }
    })

    send_request(sign_up_request)

    # Example request to sign in
    # sign_in_request = json.dumps({
    #     "type": "SIGN_IN",
    #     "data": {
    #         "username": "user",
    #         "password": "user"
    #     }
    # })
    # send_request(sign_in_request)
