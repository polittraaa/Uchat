#include "db.h"

static int validate_password(const char *password) {
    int has_upper = 0, has_lower = 0, has_digit = 0, has_special = 0;
    int length = 0;
    char special[] = {'!','@','#','$','*','-','_','?','.'};
    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') has_upper = 1;
        else if (password[i] >= 'a' && password[i] <= 'z') has_lower = 1;
        else if (password[i] >= '0' && password[i] <= '9') has_digit = 1;
        for (int j = 0; j < 9; j++) { 
            if (password[i] == special[j]) has_special = 1;
        }
        length++;
    }
    if (length < 8) return 0; 

    return has_upper && has_lower && has_digit && has_special;    
}

static int validate_number(const char *phone_number) {
    if (phone_number[0] != '+' || mx_strlen(phone_number) > 15) return 0;
    return 1;
}

int account_registration(sqlite3 *db, t_add_user *user_data) {
    char *sql = "INSERT INTO user (user_id, username, phone, email, password, user_ava) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    
    //validation
    if (!validate_password(user_data->password)) {
        fprintf(stderr, "Password does not meet criteria.\n");
        return -3;
    }
    if (mx_strcmp(user_data->email, "") == 0 && mx_strcmp(user_data->phone, "") == 0) {
        fprintf(stderr, "Either email or phone must be provided.\n");
        return -4;
    }
    if (!validate_number(user_data->phone)) {
         fprintf(stderr, "Invalid phone format.\n");
        return -5;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
         fprintf(stderr, "Failed to prepare statement: %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Bind text values from the user struct
    sqlite3_bind_text(stmt, 1, user_data->user_id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user_data->username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user_data->phone, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user_data->email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user_data->password, -1, SQLITE_STATIC);

    //photo handling
    if (mx_strlen(user_data->photo)> 0) { 
        bind_photo(stmt, 6, user_data->photo);
    }
    else {
        sqlite3_bind_null(stmt, 6);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db)); 

        sqlite3_finalize(stmt);

        if (mx_strcmp("NOT NULL constraint failed: users.username",sqlite3_errmsg(db)) == 0) {
            fprintf(stderr, "Empty Username field.\n");
            sqlite3_finalize(stmt);
            return -1;
        } else if (is_data_in_table(db, "user", "username", user_data->username) == -2) {
            fprintf(stderr, "Username already taken. %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return -2;
        } else if (is_data_in_table(db, "user", "email",  user_data->email) == -2) {
            fprintf(stderr, "Email is already registered.\n");
            sqlite3_finalize(stmt);
            return -7;
        } else if (is_data_in_table(db, "user", "phone", user_data->phone) == -2) {
            fprintf(stderr, "Phone number is already registered.\n");
            sqlite3_finalize(stmt);
            return -8;
        }
        else {
            fprintf(stderr, "Internal server error. Please try again later.\n");
            sqlite3_finalize(stmt);
            return -9;
        }
    }

    sqlite3_finalize(stmt);
    mx_printstr("Account created successfully.\n");
    return 0;
}

int get_profile(sqlite3 *db, t_add_user *user_data, t_add_settings *user_settings, t_user_request *user_id) {
    if (!db || !user_data || !user_settings || !user_id || mx_strlen(user_id->user_id) == 0) {
        fprintf(stderr, "Missing or invalid request data.\n");
        return -2;
    }
    sqlite3_stmt *stmt;
    char *sql = "SELECT u.username, u.email, u.phone, u.user_ava, s.theme "
                "FROM user u "
                "LEFT JOIN settings s ON u.user_id = s.user_id "
                "WHERE u.user_id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK ) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -3;
    }

    if (sqlite3_bind_text(stmt, 1, user_id->user_id, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind user_id: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -3;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        mx_strncpy(user_data->username, (const char *)sqlite3_column_text(stmt, 0), sizeof(user_data->username) - 1);
        mx_strncpy(user_settings->theme, (const char *)sqlite3_column_text(stmt, 1), sizeof(user_settings->theme) - 1);
        mx_strncpy(user_data->email, (const char *)sqlite3_column_text(stmt, 2), sizeof(user_data->email) - 1);
        mx_strncpy(user_data->phone, (const char *)sqlite3_column_text(stmt, 3), sizeof(user_data->phone) - 1);
    
    //photo handling
     if (mx_strlen(user_settings->photo) > 0) {
        bind_photo(stmt, 4, user_settings->photo);
    }
    else {
        sqlite3_bind_null(stmt, 4);
    }

    } else {
        fprintf(stderr, "User not found.\n");
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}
