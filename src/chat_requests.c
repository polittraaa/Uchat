
#include "db.h"

int add_new_chat(sqlite3 *db, add_chat *chat_data) { 
    char *sql = "INSERT INTO users VALUES (?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, chat_data->chat_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, chat_data->chat_name, -1, SQLITE_STATIC);

    if (is_data_in_table(db,"chat", "chat_id", chat_data->chat_id) || mx_strcmp(chat_data->chat_id, "") == 0) {
        fprintf(stderr, "Invalid or missing user_id. %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -2;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int add_user_to_chat(sqlite3 *db, add_user_chat *chat_user_data) {
    char *sql = "INSERT INTO chat_user VALUES (?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, chat_user_data->chat_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, chat_user_data->user_id, -1, SQLITE_STATIC);
 
    sqlite3_finalize(stmt);
    return 0;
}

