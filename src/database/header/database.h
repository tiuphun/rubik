#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string.h>
#include <vector>

using namespace std;

class Database {
public:
    static int db_init(const char *db_name, sqlite3 **db);
    static int db_exec(sqlite3 *db, const char *sql);
    static void db_close(sqlite3 *db);
    static int db_prepare_exec(sqlite3 *db, const char *sql, const vector<string> &params);
};

#endif // DATABASE_H
