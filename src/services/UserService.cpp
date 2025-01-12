#include "UserService.h"
#include "../database/queries/Query.h"
#include "../messages/MessageHandler.h"
#include "../models/header/EntityManager.h"
#include "openssl/sha.h"
#include <string>
#include <string.h>
#include <iostream>


using namespace std;

UserService::UserService(PlayerRepository& playerRepo, 
                        AdminRepository& adminRepo,
                        AuthRepository& authRepo,
                        EntityManager& entityManager,
                        PlayerService& playerService,
                        AdminService& adminService)
    : playerRepo(playerRepo), adminRepo(adminRepo), authRepo(authRepo), 
        entityManager(entityManager), playerService(playerService), adminService(adminService) {}

json UserService::signUp(const string& username, const string& password) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);

    bool checkUsernameTaken = authRepo.isUsernameTaken(username);
    if(checkUsernameTaken){
        return MessageHandler::craftResponse("error", {{"message", "Username is already taken"}});   
    }else{
        bool result = playerRepo.registerPlayer(username,password_hash);
        if(result){
            return MessageHandler::craftResponse("success", {{"message", "Player created successfully"}});
        }else{
            return MessageHandler::craftResponse("error", {{"message", "Failed to create the player in the database"}});
        }
    }  
}

json UserService::signIn(const string& username, const string& password, int client_socket) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);

    bool checkPlayerBanned = authRepo.isPlayerBanned(username);
    if(checkPlayerBanned){
        return MessageHandler::craftResponse("error", {{"message", "Player is banned"}}); 
    }
    auto authResult = authRepo.validateCredentials(username, password_hash);
    if(!authResult){
        return MessageHandler::craftResponse("error", {{"message", "Wrong username or password"}}); 
    }

    if (authResult->user_type == "PLAYER"){
        if(authResult->account_status == "BANNED"){
            return MessageHandler::craftResponse("error", {{"message", "Player is banned!"}}); 
        }else if(authResult->account_status == "ACTIVE"){
            return MessageHandler::craftResponse("error", {{"message", "Player is online"}}); 
        }else{
             //IF NOT BANNED or ACTIVE, find the player details, create a new player under EntityManager. Then update the status to the database
             int player_id = authResult->id;
             Player player = playerRepo.getPlayerById(player_id);
             //Make a playerPtr and add it to entityManager
             auto playerPtr = make_unique<Player>(player);
             entityManager.addPlayer(move(playerPtr));
             if(playerRepo.connectPlayerStatusUpdate(player_id)){
                cout << "Player with id: " << player_id << "has updated the status to ACTIVE";
             }else{
                cerr << "Failed to update player ACTIVE status for player with id: " << player_id;
             }
             playerService.updatePlayerSocket(authResult->id, client_socket); // update socket fd for player here
        }
       
    } else if (authResult->user_type == "ADMIN"){
       
        int admin_id = authResult->id;
        Admin admin = adminRepo.getAdminById(admin_id);

        //Make a adminPtr and add it to entityManager
        auto adminPtr = make_unique<Admin>(admin);
        entityManager.addAdmin(move(adminPtr));

        if(adminRepo.updateAdminLastLogin(admin_id)){
            cout << "Admin with id: " << admin_id << "has updated last_login to datetime(now)";
        }else{
            cerr << "Failed to update last login for admin with id: " << admin_id;
        }
        adminService.updateAdminSocket(authResult->id, client_socket); // update socket fd for admin here
    }
}