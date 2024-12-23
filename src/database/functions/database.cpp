#include <stdio.h>
#include <string.h>
#include "database.h"
#include <sqlite3.h>
#include <vector>
#include <string>

using namespace std;

// rc: Return code
int Database::db_init(const char *db_name, sqlite3 **db) {
    int rc = sqlite3_open(db_name, db);

    if (rc) {
        fprintf(stderr, "ERR: Cannot open sqlite connection. %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    fprintf(stdout, "SQLITE server connected!\n");
    fprintf(stdout, "DB name: %s\n", db_name);
    return SQLITE_OK;
}

int Database::db_exec(sqlite3 *db, const char *sql) {
    char *zErrMsg = 0;

    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERR: SQL Command execution failed. %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return rc;
}

int Database::db_prepare_exec(sqlite3 *db, const char *sql, const vector<string> &params) {
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); // Prepare the SQL statement
    if (rc != SQLITE_OK) {
        fprintf(stderr, "ERR: Failed to Prepare Statement. %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Bind parameters
    for (size_t i = 0; i < params.size(); ++i) {
        rc = sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "ERR: Failed to bind parameter %zu: %s\n", i + 1, sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Process rows for SELECT queries
        int num_cols = sqlite3_column_count(stmt);
        fprintf(stdout, "Results:\n");
        while (rc == SQLITE_ROW) {
            for (int col = 0; col < num_cols; ++col) {
                const char *col_value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, col));
                fprintf(stdout, "%s: %s\t", sqlite3_column_name(stmt, col), (col_value ? col_value : "NULL"));
            }
            fprintf(stdout, "\n");
            rc = sqlite3_step(stmt);
        }
    } else if (rc == SQLITE_DONE) {
        fprintf(stdout, "Statement executed successfully.\n");
    } else {
        fprintf(stderr, "ERR: Execution failed: %s\n", sqlite3_errmsg(db));
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE || rc == SQLITE_ROW) ? SQLITE_OK : rc;
}

void Database::db_close(sqlite3 *db) {
    sqlite3_close(db);
    fprintf(stdout, "SQLite database closing...\n");
}
