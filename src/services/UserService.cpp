#include "UserService.h"
#include "AdminService.h"
#include "PlayerService.h"
#include "../database/queries/Query.h"
#include "../messages/MessageCrafter.h"
#include "../models/header/EntityManager.h"
#include "../include/openssl/sha.h"

#include <string>
#include <string.h>
#include <iostream>

using namespace std;

nlohmann::json UserService::signUp(const string& username, const string& password) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    printf("Hashed password: %s\n", password_hash.c_str());

    // Test response
    string user_type;
    if (username == "admin") user_type = "ADMIN";
    else user_type = "PLAYER";
    nlohmann::json test = MessageCrafter::craftResponse("success", {{"message", "Your account is created"}, {"user_type", user_type}});
    printf("Response: %s\n", test.dump().c_str());
    return test; 

    // All the code below is not running
    bool checkUsernameTaken = authRepo.isUsernameTaken(username);
    printf("Username taken? %d\n", checkUsernameTaken);

    if(checkUsernameTaken){
        return MessageCrafter::craftResponse("error", {{"message", "Username is already taken"}});   
    }else{
        bool result = playerRepo.registerPlayer(username,password_hash);
        if(result){
            return MessageCrafter::craftResponse("success", {{"message", "Player created successfully"}});
        }else{
            return MessageCrafter::craftResponse("error", {{"message", "Failed to create the player in the database"}});
        }
    }  
}

nlohmann::json UserService::signIn(const string& username, const string& password, int client_socket) {
    // Hash the password
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password.c_str(), password.length(), hash);
    string password_hash(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    // printf("Hashed password: %s\n", password_hash.c_str());

    // This is just for testing. No check at all
    // string user_type;
    // if (username == "admin") user_type = "ADMIN";
    // else user_type = "PLAYER";
    // nlohmann::json test = MessageCrafter::craftResponse("success", {{"message", "You are signed in"}, {"user_type", user_type}});
    // printf("Response: %s\n", test.dump().c_str());
    // return test;

    // All the code below is not running
    bool checkPlayerBanned = authRepo.isPlayerBanned(username);
    printf("Player banned? %d\n", checkPlayerBanned);
    if(checkPlayerBanned){
        nlohmann::json response = MessageCrafter::craftResponse("error", {{"message", "Player is banned"}});
        printf("Response: %s\n", response.dump().c_str());
        return response;
    }

    auto authResult = authRepo.validateCredentials(username, password_hash);
    if (authResult->user_type == "PLAYER"){
        if(authResult->account_status == "BANNED"){
            nlohmann::json response = MessageCrafter::craftResponse("error", {{"message", "Player is banned!"}});
            printf("Response: %s\n", response.dump().c_str());
            return response;
        }else if(authResult->account_status == "ACTIVE"){
            nlohmann::json response = MessageCrafter::craftResponse("error", {{"message", "Player is online"}});
            printf("Response: %s\n", response.dump().c_str());
            return response;
        }else{
             //IF NOT BANNED or ACTIVE, find the player details, create a new player under EntityManager. Then update the status to the database
             int player_id = authResult->id;
             Player player = playerRepo.getPlayerById(player_id);
             //Make a playerPtr and add it to entityManager
             auto playerPtr = make_unique<Player>(player);
             entityManager.addPlayer(std::move(playerPtr));
             if(playerRepo.connectPlayerStatusUpdate(player_id)){
                cout << "Player with id: " << player_id << " has updated the status to ACTIVE" << endl;
             }else{
                cerr << "Failed to update player ACTIVE status for player with id: " << player_id << endl;
             }
             playerService.updatePlayerSocket(authResult->id, client_socket); // update socket fd for player here
             nlohmann::json response = MessageCrafter::craftResponse("success", {{"message", player_id}});
             printf("Response: %s\n", response.dump().c_str());
             return response;
        }
       
    } else if (authResult->user_type == "ADMIN"){
       
        int admin_id = authResult->id;
        Admin admin = adminRepo.getAdminById(admin_id);

        //Make a adminPtr and add it to entityManager
        auto adminPtr = make_unique<Admin>(admin);
        entityManager.addAdmin(std::move(adminPtr));

        if(adminRepo.updateAdminLastLogin(admin_id)){
            cout << "Admin with id: " << admin_id << " has updated last_login to datetime(now)" << endl;
        }else{
            cerr << "Failed to update last login for admin with id: " << admin_id << endl;
        }
        adminService.updateAdminSocket(authResult->id, client_socket); // update socket fd for admin here

        nlohmann::json response = MessageCrafter::craftResponse("success", {{"message", admin_id}});
        printf("Response: %s\n", response.dump().c_str());
        return response;
    }
    // nlohmann::json response = MessageCrafter::craftResponse("error", {{"message", "Wrong username or password"}});
    // printf("Response: %s\n", response.dump().c_str());
    // return response;
}