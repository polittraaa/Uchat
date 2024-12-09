#include "db.h"

void print_message(const t_add_message *msg) {
    printf("Message ID: %s\n", msg->message_id);
    printf("Chat ID: %s\n", msg->chat_id);
    printf("Timestamp: %ld\n", msg->timestamp);
    printf("Send To ID: %s\n", msg->send_to_id);
    printf("Send From ID: %s\n", msg->send_from_id);
    printf("Message: %s\n", msg->message);
    if (msg->binary) {
        printf("Binary Data: %d bytes\n", mx_strlen((const char *)msg->binary));
    } else {
        printf("Binary Data: NULL\n");
    }
}

int test_message_functions(sqlite3 *db) {

    if (sqlite3_open("messanger.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Failed to open in-memory database: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Test data
    t_add_message test_message = {
        .message_id = "msg1",
        .chat_id = "chat1",
        .timestamp = time(NULL),
        .send_to_id = "user1",
        .send_from_id = "user2",
        .message = "Hello, this is a test message!",
        .binary = (char *)"Sample binary data",
    };

    // Save the message
    printf("Saving message...\n");
    if (save_message(db, &test_message) != 0) {
        fprintf(stderr, "Failed to save message.\n");
        sqlite3_close(db);
        return -1;
    }
    printf("Message saved successfully.\n");

    // Retrieve the message
    t_add_message retrieved_message = {0};
    t_user_request sender = { .user_id = "user2" };

    printf("Retrieving message...\n");
    if (get_chat_message(db, &retrieved_message, &sender, "chat1") != 0) {
        fprintf(stderr, "Failed to retrieve message.\n");
        sqlite3_close(db);
        return -1;
    }
    printf("Message retrieved successfully:\n");
    print_message(&retrieved_message);

    // Delete the message
    t_del_mess delete_request = { .message_id = "msg1" };
    printf("Deleting message...\n");
    if (delete_message(db, &delete_request) != 0) {
        fprintf(stderr, "Failed to delete message.\n");
        sqlite3_close(db);
        return -1;
    }
    printf("Message deleted successfully.\n");

    // Verify deletion
    printf("Verifying deletion...\n");
    if (get_chat_message(db, &retrieved_message, &sender, "chat1") == 0) {
        fprintf(stderr, "Message was not deleted properly.\n");
        sqlite3_close(db);
        return -1;
    }
    printf("Deletion verified. Message no longer exists.\n");

    // Clean up
    sqlite3_close(db);
    return 0;
}
