#include "db.h"

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("messanger.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n",sqlite3_errmsg(db));
        return 1;
    }

    init_tables(db);
    mx_printstr("\n\n");
    //tests 
    //test_account_registration(db);
    test_settings_saving(db);
    mx_printstr("\n");
    test_message_functions(db);
    mx_printstr("\n");
    test_create_new_chat(db);
    mx_printstr("\n");
    sqlite3_close(db);
    return 0;
}
