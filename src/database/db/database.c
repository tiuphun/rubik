#include <stdio.h>
#include <string.h>
#include "db.h"
#include <sqlite3.h>

//rc: Return code

int db_init(const char *db_name, sqlite3 ** db){
    int rc = sqlite3_open(db_name,db);

    if(rc){
        fprintf(stderr, "ERR: Cannot open sqlite connection. %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    printf("SQLITE server connected! \n");
    printf("DB name: %s",db_name);
    return SQLITE_OK;
}

int db_exec(sqlite3 *db, const char *sql){
    char *zErrMsg = 0;

    int rc = sqlite3_exec(db,sql,0,0, &zErrMsg);
    if(rc != SQLITE_OK){
        fprintf(stderr, "ERR: SQL Command execution failed. %s\n",zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return rc;
}

void db_close(sqlite3 *db){
    sqlite3_close(db);
    printf("SQLite database closing...");
}
