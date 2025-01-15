#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include <../../include/nlohmann/json.hpp>
#include <sqlite3.h>
#include "../services/UserService.h"
#include "../services/PlayerService.h"
#include "../services/AdminService.h"
#include "../services/GameService.h"
#include "../services/RoomService.h"
#include "../repositories/AuthRepository.h"
#include "../repositories/PlayerRepository.h"
#include "../repositories/AdminRepository.h"
#include "../models/header/EntityManager.h"

using json = nlohmann::json;

class MessageHandler {
public:
     MessageHandler(sqlite3* db) 
        : entityManager(std::make_unique<EntityManager>())
        , authRepo(db)
        , playerRepo(db)
        , adminRepo(db)
        , userService(userService)
        , playerService(*entityManager, playerRepo)
        , adminService(adminRepo, playerRepo, *entityManager)
        , roomService(*entityManager)
        , gameService(*entityManager) {}

    json parseMessage(const std::string& message);
    json handleMessage(const json& parsed_message, sqlite3* db, int client_socket);
    json craftResponse(const std::string& status, const json& data);
private:
    std::unique_ptr<EntityManager> entityManager;
    AuthRepository authRepo;
    PlayerRepository playerRepo;
    AdminRepository adminRepo;
    
    // Services
    UserService userService;
    PlayerService playerService; 
    AdminService adminService;
    RoomService roomService;
    GameService gameService;

     json handleSignUp(const json& parsed_message, sqlite3* db);
     json handleSignIn(const json& parsed_message, sqlite3* db, int client_socket);
     json handleCreateRoom(const json& parsed_message, sqlite3* db);
     json handleJoinRoom(const json& parsed_message, sqlite3* db);
     json handleLeaveRoom(const json& parsed_message, sqlite3* db);
    //  json handleReady(const json& parsed_message, sqlite3* db);
     json handleStartGame(const json& parsed_message, sqlite3* db);
     json handleCube(const json& parsed_message, sqlite3* db);
     json handleEndGame(const json& parsed_message, sqlite3* db);
     json handleViewUsers(const json& parsed_message, sqlite3* db);
     json handleViewRooms(const json& parsed_message, sqlite3* db);
     json handleBanPlayer(const json& parsed_message, sqlite3* db);
     json handleSpectate(const json& parsed_message, sqlite3* db); 
};

#endif