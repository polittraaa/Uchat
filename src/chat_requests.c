
#include "db.h"

int create_new_chat(sqlite3 *db, t_add_chat *chat_data) { 
    char *sql = "INSERT INTO chat VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, chat_data->chat_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, chat_data->chat_name, -1, SQLITE_STATIC);

    if (mx_strlen(chat_data->photo) > 0) {
        sqlite3_bind_text(stmt, 3, chat_data->photo, -1, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 3);
    }

    if (is_data_in_table(db, "chat", "chat_id", chat_data->chat_id) || mx_strcmp(chat_data->chat_id, "") == 0) {
        fprintf(stderr, "Invalid or missing user_id. %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -2;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int delete_chat(sqlite3 *db, t_del_chat *chat) {
    // Step 1: Delete all users from the chat
    if (delete_users_from_chat(db, chat->chat_id) != 0) {
        fprintf(stderr, "Failed to delete users associated with chat ID: %s\n", chat->chat_id);
        return -1;
    }

    // Step 2: Delete the chat itself
    const char *sql = "DELETE FROM chat WHERE chat_id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, chat->chat_id, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind chat_id: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -2;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to delete chat: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -3;
    }

    sqlite3_finalize(stmt);
    return 0;
}
