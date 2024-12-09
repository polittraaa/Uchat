#include "db.h"

void test_settings_saving(sqlite3 *db) {

    if (sqlite3_open("messanger.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Test case 1: Successful registration
    t_add_settings settings1 = { .user_id = "user1", .theme = "light", .email = "user1@example.com", .phone = "+1234567890565", .photo = "path/to/photo1.jpg" };
    t_add_settings settings2 = { .user_id = "user2", .theme = "dark", .email = "user2@example.com", .phone = "+1234567890334", .photo = "" };
    t_add_settings settings3 = { .user_id = "user3", .theme = "blue", .email = "user3@example.com", .phone = "+123456789012", .photo = "path/to/photo3.jpg" };
    t_add_settings settings4 = { .user_id = "user4", .theme = "dark", .email = "", .phone = "", .photo = "" };

    // Test case 1: Valid input with all fields
    printf("Test Case 1: ");
    int result = save_settings(db, &settings1);
    printf("Expected: 0, Got: %d\n", result);

    // Test case 2: Valid input with NULL photo
    printf("Test Case 2: ");
    result = save_settings(db, &settings2);
    printf("Expected: 0, Got: %d\n", result);

    // Test case 3: Invalid theme
    printf("Test Case 3: ");
    result = save_settings(db, &settings3);
    printf("Expected: -2, Got: %d\n", result);

    // Test case 4: Valid input with empty email and photo
    printf("Test Case 4: ");
    result = save_settings(db, &settings4);
    printf("Expected: -4, Got: %d\n", result);

    sqlite3_close(db);
}

void test_return_settings(sqlite3 *db) {
    // Open database connection
    if (sqlite3_open("messenger.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Prepare test cases
    t_get_settings test_user1 = { .user_id = "user1" };
    t_get_settings test_user2 = { .user_id = "user2" };
    t_get_settings test_user3 = { .user_id = "nonexistent_user" };

    t_add_settings retrieved_settings;

    // Test case 1: Valid user with full settings
    printf("Test Case 1: Valid user with full settings\n");
    int result = return_settings(db, &test_user1, &retrieved_settings);
    printf("Expected: 0, Got: %d\n", result);
    if (result == 0) {
        printf("Retrieved Settings:\n");
        printf("  User ID: %s\n", retrieved_settings.user_id);
        printf("  Theme: %s\n", retrieved_settings.theme);
        printf("  Email: %s\n", retrieved_settings.email);
        printf("  Phone: %s\n", retrieved_settings.phone);
        printf("  Photo: %s\n", retrieved_settings.photo);
    }

    // Test case 2: Valid user with partial settings
    printf("Test Case 2: Valid user with partial settings\n");
    result = return_settings(db, &test_user2, &retrieved_settings);
    printf("Expected: 0, Got: %d\n", result);
    if (result == 0) {
        printf("Retrieved Settings:\n");
        printf("  User ID: %s\n", retrieved_settings.user_id);
        printf("  Theme: %s\n", retrieved_settings.theme);
        printf("  Email: %s\n", retrieved_settings.email);
        printf("  Phone: %s\n", retrieved_settings.phone);
        printf("  Photo: %s\n", retrieved_settings.photo);
    }

    // Test case 3: Nonexistent user
    printf("Test Case 3: Nonexistent user\n");
    result = return_settings(db, &test_user3, &retrieved_settings);
    printf("Expected: -3, Got: %d\n", result);
    if (result == -3) {
        printf("No matching records found for user ID: %s\n", test_user3.user_id);
    }

    // Close database connection
    sqlite3_close(db);
}
