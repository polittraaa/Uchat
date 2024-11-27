#include "db.h"

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("messanger.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n",sqlite3_errmsg(db));
        return 1;
    }

    if (init_tables(db) != 0) {
        fprintf(stderr, "Failed to initialize tables.\n");
        sqlite3_close(db);
        return 1;
    }
   // test_insert_data(db);

    // add_settings user_settings;
    // mx_strncpy(user_settings.theme, "dark_mode", sizeof(user_settings.theme));
    // mx_strncpy(user_settings.email, "user@example.com", sizeof(user_settings.email));
    // mx_strncpy(user_settings.photo, "https://example.com/photo.jpg", sizeof(user_settings.photo));

    test_account_registration(db);
    
    // int result = save_settings(db, &user_settings);
    // if (result == 0) {
    //      mx_printstr("Settings saved successfully.\n");
    // } else {
    //     mx_printstr("Failed to save settings.\n");
    // }

    sqlite3_close(db);

    return 0;
}
