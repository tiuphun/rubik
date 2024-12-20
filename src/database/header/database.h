#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

/**
 * Hàm mở kết nối đến SQLite
 * Tham số: Tên database và object sqlite3 để hold connection
 */
int db_init(const char *db_name, sqlite3 ** db);
/**
 * Hàm thực thi một SQL query
 * Tham số: sqlite3 object và SQL query
 */
int db_exec(sqlite3 *db, const char *sql);
/**
 * Hàm đóng kết nối SQLite database
 * Tham số: sqlite3 ọbject
 */
void db_close(sqlite3 *db);

#endif 