#pragma once

#include "/home/poli3/UCODE_UCHAT/Code/client/libs/libmx/inc/libmx.h"
#include <stdio.h>
#include <sqlite3.h>
#include <time.h>
//#include "logger.h"
//#include "cencode.h"

// Structures 
    typedef struct {
        char username[64];
        char user_id[64];
        char password[32];
        char email[128];
        char phone[20];
        char photo[256];
    } t_add_user;

    typedef struct {
        char username[64];
        char theme[16];
        char email[128];
        char phone[20];
        char photo[256];
    } t_get_user;

    typedef struct {
        char user_id[64];
    } t_user_request;

    typedef struct {
        char chat_id[64]; 
        char chat_name[64];
        char *photo;
    } t_add_chat, t_del_chat;

    typedef struct {
        char chat_id[64]; 
        char user_id[64]; 
    } t_add_user_chat;

    typedef struct {
        char message_id[64];
        char chat_id[64];
        time_t timestamp;
        char send_to_id[64];
        char send_from_id[64];
        char message[512];
        char *binary;
    } t_add_message, t_del_mess;

    typedef struct {
        char user_id[64];
        char username[64];
    } t_get_settings;

    typedef struct {
        char user_id[64];
        char theme[16];
        char email[128];
        char phone[20]; 
        char photo[256];
    } t_add_settings;

    typedef struct {
    char *username;
    char *photo;
    } t_get_contacts;



//db
int init_tables(sqlite3 *db);

void bind_photo(sqlite3_stmt *stmt, int row,  char *photo);
int is_data_in_table(sqlite3 *db, const char *table_name, const char *column_name, const char *value);

//Saving to db
int account_registration(sqlite3 *db, t_add_user *user_data);
int save_settings(sqlite3 *db, t_add_settings *user_settings);
int create_new_chat(sqlite3 *db, t_add_chat *chat_data);
int save_message(sqlite3 *db, t_add_message *new_message);
int add_user_to_chat(sqlite3 *db, t_add_user_chat *chat_user_data);

//Returning from base 
int return_settings(sqlite3 *db, t_get_settings *user, t_add_settings *settings);
int get_profile(sqlite3 *db, t_add_user *user_data, t_add_settings *user_settings, t_user_request *user_id);
int get_chat_message(sqlite3 *db, t_add_message *msg, t_user_request *sender, char *chat_id);

//Delete from base 
int delete_chat(sqlite3 *db, t_del_chat *chat);
int delete_message(sqlite3 *db, t_del_mess *msg);
int delete_users_from_chat(sqlite3 *db, const char *chat_id);

//temporry test 
void test_account_registration(sqlite3 *db);
void test_settings_saving(sqlite3 *db);
int test_message_functions(sqlite3 *db);
void test_create_new_chat(sqlite3 *db);

//int mx_base64_decode(const char *encoded_str, char **decoded_str);
//int mx_base64_encode(const char *input_str, char **encoded_str);
