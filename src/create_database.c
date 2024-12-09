#include "db.h"

int init_tables(sqlite3 *db) {
    //  User
    const char *sql_create_user =
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id TEXT PRIMARY KEY,"
        "username TEXT NOT NULL UNIQUE,"
        "phone TEXT UNIQUE,"
        "email TEXT UNIQUE,"
        "password TEXT NOT NULL,"
        "user_ava BLOB"
        ");";

    // Chat
    const char *sql_create_chat =
        "CREATE TABLE IF NOT EXISTS chat ("
        "chat_id TEXT PRIMARY KEY," 
        "chat_name TEXT NOT NULL,"
        "chat_ava BLOB"
        ");";

    //  Message
    const char *sql_create_message =
        "CREATE TABLE IF NOT EXISTS message ("
        "message_id TEXT PRIMARY KEY,"
        "chat_id TEXT NOT NULL,"
        "timestamp DATETIME NOT NULL,"
        "send_to_id TEXT NOT NULL,"
        "send_from_id TEXT NOT NULL,"
        "message TEXT,"
        "binary BLOB,"
        "FOREIGN KEY(send_to_id) REFERENCES user(user_id),"
        "FOREIGN KEY(send_from_id) REFERENCES chat(chat_id)"
        ");";

    //Settings
    const char *sql_create_settings =
        "CREATE TABLE IF NOT EXISTS settings ("
        "theme TEXT NOT NULL,"
        "user_id TEXT,"
        "email TEXT,"
        "phone TEXT,"
        "photo BLOB,"
        "FOREIGN KEY(phone) REFERENCES user(phone),"
        "FOREIGN KEY(email) REFERENCES user(email),"
        "FOREIGN KEY(photo) REFERENCES user(user_ava)" 
        ");";
        
    //Chat and User conection     
    const char *sql_create_chat_user =
        "CREATE TABLE IF NOT EXISTS chat_user ("
        "chat_id TEXT NOT NULL,"
        "user_id TEXT NOT NULL," 
        "FOREIGN KEY(chat_id) REFERENCES chat(chat_id),"
        "FOREIGN KEY(user_id) REFERENCES user(user_id),"
        "UNIQUE(chat_id, user_id)"
        ");";

    const char *sql_create_contacts  =
        "CREATE TABLE IF NOT EXISTS user ("
        "user_id TEXT NOT NULL,"
        "contact_id TEXT PRIMARY KEY,"
        "contact_name TEXT NOT NULL, "
        "contact_info TEXT, "
        "FOREIGN KEY(user_id) REFERENCES user(user_id)"
        ");";

    // LastSeen
    // const char *sql_create_lastseen =
    // "CREATE TABLE IF NOT EXISTS last_seen ("
    // "user_id TEXT NOT NULL,"
    // "timestamp DATETIME NOT NULL,"
    // "FOREIGN KEY(user_id) REFERENCES User(user_id)"
    // ");";

    const char *statements[] = {
        sql_create_user,
        sql_create_chat,
        sql_create_message,
        sql_create_settings,
        sql_create_chat_user,
        sql_create_contacts,
    };

    // Execute each statement and check for errors
    for (int i = 0; i < (int)sizeof(statements) / (int)sizeof(statements[0]); i++) {
        char *errmsg = NULL;
        if (sqlite3_exec(db, statements[i], NULL, NULL, &errmsg) != SQLITE_OK) {
            fprintf(stderr, "SQL Error in statement %d: %s\nStatement: %s\n", i + 1, errmsg, statements[i]);
            sqlite3_free(errmsg);
            fprintf(stderr, "Failed to initialize tables.\n");
            return -1;
        }
    }

    printf("All tables initialized successfully.\n");

    sqlite3_close(db);
    return 0;
}

