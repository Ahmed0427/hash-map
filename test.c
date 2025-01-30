#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "map.h"

void test_map_init() {
    Map map;
    map_init(&map);
    assert(map.size == 0);
    assert(map.capacity > 0);
    printf("map_init passed.\n");
}

void test_map_insert() {
    Map map;
    map_init(&map);

    int value1 = 42;
    assert(map_insert(&map, "key1", &value1, sizeof(value1)));

    int* retrieved_value1 = (int*) map_get(&map, "key1");
    assert(retrieved_value1 != NULL && *retrieved_value1 == value1);
    printf("map_insert passed.\n");
}

void test_map_erase() {
    Map map;
    map_init(&map);

    int value1 = 42;
    map_insert(&map, "key1", &value1, sizeof(value1));

    assert(map_erase(&map, "key1"));
    assert(map_get(&map, "key1") == NULL);
    printf("map_erase passed.\n");
}

void test_map_size() {
    Map map;
    map_init(&map);

    int value1 = 42;
    map_insert(&map, "key1", &value1, sizeof(value1));
    assert(map_size(&map) == 1);

    int value2 = 100;
    map_insert(&map, "key2", &value2, sizeof(value2));
    assert(map_size(&map) == 2);
    printf("map_size passed.\n");
}

void test_map_free() {
    Map map;
    map_init(&map);

    int value1 = 42;
    map_insert(&map, "key1", &value1, sizeof(value1));

    map_free(&map);
    // No assert here, but ensure no crash or memory leak
    printf("map_free passed.\n");
}

void test_map_get() {
    Map map;
    map_init(&map);

    int value1 = 42;
    map_insert(&map, "key1", &value1, sizeof(value1));

    int* retrieved_value1 = (int*) map_get(&map, "key1");
    assert(retrieved_value1 != NULL && *retrieved_value1 == value1);

    int value2 = 100;
    map_insert(&map, "key2", &value2, sizeof(value2));
    int* retrieved_value2 = (int*) map_get(&map, "key2");
    assert(retrieved_value2 != NULL && *retrieved_value2 == value2);

    printf("map_get passed.\n");
}

int main() {
    test_map_init();
    test_map_insert();
    test_map_erase();
    test_map_size();
    test_map_free();
    test_map_get();

    printf("All tests passed!\n");
    return 0;
}

