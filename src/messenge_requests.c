
#include "db.h"

int save_message(sqlite3 *db, t_add_message *new_message) {
    char *sql = "INSERT INTO message VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, new_message->message_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_message->chat_id, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, new_message->timestamp);
    sqlite3_bind_text(stmt, 4, new_message->send_to_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, new_message->send_from_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, new_message->message, -1, SQLITE_STATIC);
    
    //photo handling
    if (new_message->binary != NULL) {
        bind_photo(stmt, 7, new_message->binary);
    }
    else {
        sqlite3_bind_null(stmt, 7);
    }

    return 0;
}

int get_chat_message(sqlite3 *db, t_add_message *msg, t_user_request *sender, char *chat_id) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM message WHERE chat_id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK ) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, sender->user_id, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind user_id: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -2;
    }

    sqlite3_bind_text(stmt, 1, chat_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, sender->user_id, -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
    mx_strncpy(msg->message_id, (const char *)sqlite3_column_text(stmt, 0), sizeof(msg->message_id) - 1);
    mx_strncpy(msg->chat_id, (const char *)sqlite3_column_text(stmt, 1), sizeof(msg->chat_id) - 1);
    msg->timestamp = (time_t)sqlite3_column_int64(stmt, 2);  // Converting timestamp to time_t

    mx_strncpy(msg->send_to_id, (const char *)sqlite3_column_text(stmt, 3), sizeof(msg->send_to_id) - 1);
    mx_strncpy(msg->send_from_id, (const char *)sqlite3_column_text(stmt, 4), sizeof(msg->send_from_id) - 1);
    mx_strncpy(msg->message, (const char *)sqlite3_column_text(stmt, 5), sizeof(msg->message) - 1);
    
   const void *binary_data = sqlite3_column_blob(stmt, 6);
    int binary_size = sqlite3_column_bytes(stmt, 6);

    if (binary_size > 0) {
        msg->binary = malloc(binary_size);
        if (msg->binary != NULL) {
           mx_memcpy(msg->binary, binary_data, binary_size);
        }
    } else {
        msg->binary = NULL; 
    }
    sqlite3_finalize(stmt);
    return 0;
    } else {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
}

int delete_message(sqlite3 *db, t_del_mess *msg) {
    sqlite3_stmt *stmt;
    char *sql = "DELETE FROM message WHERE message_id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK ) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_bind_text(stmt, 1, msg->message_id, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to delete message: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -2;
    }

    sqlite3_finalize(stmt);
    return 0;
}
