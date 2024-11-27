#include "db.h"

// void bind_photo(sqlite3_stmt *stmt, char *photo) { 
//     size_t photo_size = mx_strlen(photo);
//     unsigned char *photo_data = base64_decode(photo, photo_size, &photo_size);
 
//     if (photo_data != NULL) {
//         sqlite3_bind_blob(stmt, 5, photo_data, photo_size, SQLITE_STATIC);
//         free(photo_data);
//     } else {
//         fprintf(stderr, "Failed to decode photo data.\n");
//         sqlite3_finalize(stmt);
//         return -1;
//     } 
// }

int is_data_in_table(sqlite3 *db, const char *table_name, const char *column_name, const char *value) {
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT EXISTS(SELECT 1 FROM %s WHERE %s = ? LIMIT 1);", table_name, column_name);

    sqlite3_stmt *stmt;
    int exists = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0);
    } else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        exists = -1; 
    }
    sqlite3_finalize(stmt);
    return exists;
}


int add_messege(sqlite3 *db, add_message *new_message){
    char *sql = "INSERT INTO message VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, new_message->message_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_message->message_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, new_message->send_to_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, new_message->send_from_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, new_message->message, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, new_message->binary, -1, SQLITE_STATIC);

    return 0;
}



