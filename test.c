#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "map.h"

// Test helper functions
void assert_str_eq(const char* actual, const char* expected, const char* message) {
    if (strcmp(actual, expected) != 0) {
        printf("FAIL: %s\n", message);
        printf("Expected: %s\n", expected);
        printf("Actual: %s\n", actual);
        assert(0);
    }
}

void test_init() {
    printf("Testing initialization...\n");
    Map* map = map_create();
    assert(map != NULL);
    assert(map_size(map) == 0);
    map_free(map);
    printf("✓ Init tests passed\n\n");
}

void test_basic_operations() {
    printf("Testing basic operations...\n");
    Map* map = map_create();
    assert(map != NULL);

    // Test insertion
    const char* value1 = "value1";
    const char* value2 = "value2";
    assert(map_insert(map, "key1", value1, strlen(value1) + 1));
    assert(map_insert(map, "key2", value2, strlen(value2) + 1));
    assert(map_size(map) == 2);

    // Test retrieval
    char* retrieved1 = (char*)map_get(map, "key1");
    char* retrieved2 = (char*)map_get(map, "key2");
    assert_str_eq(retrieved1, "value1", "Retrieved value1 incorrect");
    assert_str_eq(retrieved2, "value2", "Retrieved value2 incorrect");

    // Test update
    const char* updated_value = "updated";
    assert(map_insert(map, "key1", updated_value, strlen(updated_value) + 1));
    retrieved1 = (char*)map_get(map, "key1");
    assert_str_eq(retrieved1, "updated", "Updated value incorrect");

    // Test deletion
    assert(map_erase(map, "key1"));
    assert(map_size(map) == 1);
    assert(map_get(map, "key1") == NULL);
    assert(map_get(map, "key2") != NULL);

    map_free(map);
    printf("✓ Basic operations tests passed\n\n");
}

void test_collisions() {
    printf("Testing hash collisions...\n");
    Map* map = map_create();
    assert(map != NULL);

    // Insert multiple items (some will likely collide)
    for (int i = 0; i < 100; i++) {
        char key[16];
        char value[16];
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        assert(map_insert(map, key, value, strlen(value) + 1));
    }

    // Verify all items
    for (int i = 0; i < 100; i++) {
        char key[16];
        char expected[16];
        sprintf(key, "key%d", i);
        sprintf(expected, "value%d", i);
        char* retrieved = (char*)map_get(map, key);
        assert_str_eq(retrieved, expected, "Collision handling failed");
    }

    map_free(map);
    printf("✓ Collision tests passed\n\n");
}

void test_edge_cases() {
    printf("Testing edge cases...\n");
    Map* map = map_create();
    assert(map != NULL);

    // Test NULL handling
    assert(!map_insert(NULL, "key", "value", 6));
    assert(!map_insert(map, NULL, "value", 6));
    assert(!map_insert(map, "key", NULL, 6));
    assert(!map_insert(map, "key", "value", 0));

    assert(map_get(NULL, "key") == NULL);
    assert(map_get(map, NULL) == NULL);

    assert(!map_erase(NULL, "key"));
    assert(!map_erase(map, NULL));

    // Test empty string
    const char* empty = "";
    assert(map_insert(map, "empty", empty, 1));
    char* retrieved = (char*)map_get(map, "empty");
    assert_str_eq(retrieved, "", "Empty string handling failed");

    // Test large values
    char large_value[1024];
    memset(large_value, 'A', 1023);
    large_value[1023] = '\0';
    assert(map_insert(map, "large", large_value, 1024));
    retrieved = (char*)map_get(map, "large");
    assert_str_eq(retrieved, large_value, "Large value handling failed");

    map_free(map);
    printf("✓ Edge case tests passed\n\n");
}

void test_resize() {
    printf("Testing map resize...\n");
    Map* map = map_create();
    assert(map != NULL);

    // Insert enough items to trigger multiple resizes
    for (int i = 0; i < 100; i++) {
        char key[16];
        char value[16];
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        assert(map_insert(map, key, value, strlen(value) + 1));
    }

    // Verify all items after resizes
    for (int i = 0; i < 100; i++) {
        char key[16];
        char expected[16];
        sprintf(key, "key%d", i);
        sprintf(expected, "value%d", i);
        char* retrieved = (char*)map_get(map, key);
        assert_str_eq(retrieved, expected, "Resize handling failed");
    }

    map_free(map);
    printf("✓ Resize tests passed\n\n");
}

void test_delete_all() {
    printf("Testing deletion of all elements...\n");
    Map* map = map_create();
    assert(map != NULL);

    // Insert and then delete all items
    for (int i = 0; i < 10; i++) {
        char key[16];
        char value[16];
        sprintf(key, "key%d", i);
        sprintf(value, "value%d", i);
        assert(map_insert(map, key, value, strlen(value) + 1));
    }

    for (int i = 0; i < 10; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        assert(map_erase(map, key));
    }

    assert(map_size(map) == 0);

    // Try inserting after deletion
    assert(map_insert(map, "new_key", "new_value", 10));
    char* retrieved = (char*)map_get(map, "new_key");
    assert_str_eq(retrieved, "new_value", "Insert after delete failed");

    map_free(map);
    printf("✓ Delete all tests passed\n\n");
}

int main() {
    printf("Running hash map tests...\n\n");

    test_init();
    test_basic_operations();
    test_collisions();
    test_edge_cases();
    test_resize();
    test_delete_all();
}
