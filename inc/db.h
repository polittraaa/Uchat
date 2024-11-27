#pragma once

#include "/home/poli3/UCODE_UCHAT/Code/client/libs/libmx/inc/libmx.h"
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>

// Structures 
    typedef struct {
        char username[64];
        char user_id[64];
        char password[32];
        char email[128];
        char phone[20];
        char photo[256];
    } add_user;

    typedef struct {
        char chat_id[64]; 
        char chat_name[64];
    } add_chat;

    typedef struct {
        char chat_id[64]; 
        char user_id[64]; 
    } add_user_chat;

    typedef struct {
        char message_id[64];
        time_t timestemp;
        char send_to_id[64];
        char send_from_id[64];
        char message[512];
        char *binary;
    } add_message;

    typedef struct {
        char user_id[64];
        char username[64];
    } user_login;

    typedef struct {
       // char *user_id;
        char theme[16];
        char email[128]; 
        char photo[256];
    } add_settings;

//db
void insert_data(sqlite3 *db);
int execute_sql(sqlite3 *db, const char *sql);
int init_tables(sqlite3 *db);
void bind_photo(sqlite3_stmt *stmt, char *photo);
//int is_duplicate(sqlite3 *db, const char *column, const char *value);
int is_data_in_table(sqlite3 *db, const char *table_name, const char *column_name, const char *value);

int account_registration(sqlite3 *db, add_user *user_data);

//temporry test 
void test_account_registration(sqlite3 *db);
int add_new_chat(sqlite3 *db, add_chat *chat_data);
