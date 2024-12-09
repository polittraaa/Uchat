#include "db.h"

void test_create_new_chat(sqlite3 *db) {
    sqlite3_open("messanger.db", &db);

    t_add_chat chat1 = { .chat_id = "chatID1", .chat_name = "Chat One", .photo = ""};
    t_add_chat chat2 = { .chat_id = "chatID1", .chat_name = "Duplicate Chat", .photo = ""};
    t_add_chat chat3 = { .chat_id = "", .chat_name = "Invalid Chat", .photo = ""};

    printf("Testing Chat creation \n");
    // Test case 1: Successful chat creation
    printf("Test Case 1: ");
    int result = create_new_chat(db, &chat1);
    printf("Expected: 0, Got: %d\n", result);

    // Test case 2: Duplicate chat ID
    printf("Test Case 2: ");
    result = create_new_chat(db, &chat2);
    printf("Expected: -2, Got: %d\n", result);

    // Test case 3: Missing chat ID
    printf("Test Case 3: ");
    result = create_new_chat(db, &chat3);
    printf("Expected: -2, Got: %d\n", result);

    sqlite3_close(db);
}
