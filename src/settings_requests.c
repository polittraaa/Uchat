#include "db.h"

int save_settings(sqlite3 *db, add_settings *user_settings) {
    sqlite3_stmt *stmt;
    char *sql = "INSERT INTO chats VALUES (?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    if ((mx_strcmp(user_settings->theme, "Dark") != 0 ) && (mx_strcmp(user_settings->theme, "Light") != 0)) 

    sqlite3_bind_text(stmt, 2, user_settings->email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 1, user_settings->theme, -1, SQLITE_STATIC);


    // if (user_settings->photo != NULL) {
    //    bind_photo(stmt, user_settings->photo);
    // }
    // else {
    //     sqlite3_bind_null(stmt, 3);
    // }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        char error[128];
        snprintf(error, sizeof(error), "Execution failed: %s\n", sqlite3_errmsg(db));
        //logger_error(error);
        sqlite3_finalize(stmt);
        return -1;
    }
    //  else {
    //     logger_info("Settings saved successfully.\n");
    // }

    return 0;
}
