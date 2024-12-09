#include "db.h"

int delete_users_from_chat(sqlite3 *db, const char *chat_id) {
    const char *sql = "DELETE FROM user_chat WHERE chat_id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, chat_id, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind chat_id: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -2;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to delete users from chat: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -3;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int add_user_to_chat(sqlite3 *db, t_add_user_chat *chat_user_data) {
    char *sql = "INSERT INTO chat_user VALUES (?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, chat_user_data->chat_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, chat_user_data->user_id, -1, SQLITE_STATIC);
 
    if (is_data_in_table(db, "chat_user", "user_id", chat_user_data->user_id) == -2) {
            fprintf(stderr, "User already in chat. %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return -2;
    }
    sqlite3_finalize(stmt);
    return 0;
}
