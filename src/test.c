#include "db.h"

void test_account_registration(sqlite3 *db) {
    sqlite3_open("messanger.db", &db);  // Using in-memory database for testing

    add_user user1 = { .username = "test_user1", .password = "Password1!", .email = "test1@example.com", .phone = "+1234567890" };
    add_user user2 = { .username = "test_user2", .password = "short", .email = "test2@example.com", .phone = "+987654321" };
    add_user user3 = { .username = "test_user3", .password = "Password1!", .email = "", .phone = "" };
    add_user user4 = { .username = "test_user1", .password = "Password1!", .email = "duplicate@example.com", .phone = "+1122334455" };

    // Test case 1: Successful registration
    printf("Test Case 1: ");
    int result = account_registration(db, &user1);
    printf("Expected: 0, Got: %d\n", result);

    // Test case 2: Invalid password
    printf("Test Case 2: ");
    result = account_registration(db, &user2);
    printf("Expected: -3, Got: %d\n", result);

    // Test case 3: Missing email and phone
    printf("Test Case 3: ");
    result = account_registration(db, &user3);
    printf("Expected: -4, Got: %d\n", result);

    // Test case 4: Duplicate username
    printf("Test Case 4: ");
    result = account_registration(db, &user4);
    printf("Expected: -2, Got: %d\n", result);

    sqlite3_close(db);
}
