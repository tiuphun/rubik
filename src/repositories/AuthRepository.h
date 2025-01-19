#ifndef AUTH_REPOSITORY_H
#define AUTH_REPOSITORY_H

#include <sqlite3.h>
#include <string>
#include <optional>

using namespace std;

struct AuthResult {
    int id;
    string user_type;        // PLAYER or ADMIN
    string account_status;   // ACTIVE, INACTIVE, or BANNED
};

class AuthRepository {
public:
    AuthRepository(sqlite3* db): db(db) {}

    bool isUsernameTaken(const string& username);
    bool isPlayerBanned(const string& username);
    optional<AuthResult> validateCredentials(
        const string& username, const string& password
        );

private:
    sqlite3* db;
};

#endif