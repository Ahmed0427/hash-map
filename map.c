#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "map.h"

#define INIT_CAPACITY 16
#define MAX_LOAD_FACTOR 0.75

typedef struct Map_Entry {
    char* key;
    void* value;
    size_t hash;
    struct Map_Entry* next;
} Map_Entry;

struct Map {
    Map_Entry** buckets;
    size_t size;
    size_t capacity;
};

Map* map_create(void) {
    Map* map = malloc(sizeof(Map));
    if (!map) return NULL;
    
    map->size = 0;
    map->capacity = INIT_CAPACITY;
    map->buckets = calloc(map->capacity, sizeof(Map_Entry*));
    
    if (!map->buckets) {
        free(map);
        return NULL;
    }
    
    return map;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static size_t get_hash(const char* key) {
    if (!key) return 0;
    
    size_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (size_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

static bool map_extend(Map* map) {
    if (!map || map->capacity >= SIZE_MAX / 2) return false;
    
    size_t new_capacity = map->capacity * 2;
    Map_Entry** new_buckets = calloc(new_capacity, sizeof(Map_Entry*));
    if (!new_buckets) return false;
    
    for (size_t i = 0; i < map->capacity; i++) {
        Map_Entry* entry = map->buckets[i];
        while (entry) {
            Map_Entry* next = entry->next;
            size_t new_index = entry->hash % new_capacity;
            entry->next = new_buckets[new_index];
            new_buckets[new_index] = entry;
            entry = next;
        }
    }
    
    free(map->buckets);
    map->capacity = new_capacity;
    map->buckets = new_buckets;
    return true;
}

static void* void_dup(const void* ptr, size_t size) {
    if (!ptr || !size) return NULL;
    void* new_ptr = malloc(size);
    if (!new_ptr) return NULL;
    memcpy(new_ptr, ptr, size);
    return new_ptr;
}

bool map_insert(Map* map, const char* key, const void* value, size_t value_size) {
    if (!map || !key || !value || !value_size) return false;

    // Check load factor and extend if necessary
    if (map->size >= (size_t)(map->capacity * MAX_LOAD_FACTOR)) {
        if (!map_extend(map)) return false;
    }

    size_t hash = get_hash(key);
    size_t index = hash % map->capacity;

    // Check for existing key
    Map_Entry* entry = map->buckets[index];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            // Update existing entry
            void* new_value = void_dup(value, value_size);
            if (!new_value) return false;
            free(entry->value);
            entry->value = new_value;
            return true;
        }
        entry = entry->next;
    }

    // Create new entry
    Map_Entry* new_entry = malloc(sizeof(Map_Entry));
    if (!new_entry) return false;

    // Copy key
    size_t key_len = strlen(key) + 1;
    new_entry->key = malloc(key_len);
    if (!new_entry->key) {
        free(new_entry);
        return false;
    }
    memcpy(new_entry->key, key, key_len);

    // Copy value
    new_entry->value = void_dup(value, value_size);
    if (!new_entry->value) {
        free(new_entry->key);
        free(new_entry);
        return false;
    }

    // Insert new entry
    new_entry->hash = hash;
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;
    map->size++;

    return true;
}

void* map_get(const Map* map, const char* key) {
    if (!map || !key) return NULL;

    size_t hash = get_hash(key);
    size_t index = hash % map->capacity;

    Map_Entry* entry = map->buckets[index];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

bool map_erase(Map* map, const char* key) {
    if (!map || !key) return false;

    size_t hash = get_hash(key);
    size_t index = hash % map->capacity;

    Map_Entry* prev = NULL;
    Map_Entry* curr = map->buckets[index];

    while (curr) {
        if (curr->hash == hash && strcmp(curr->key, key) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                map->buckets[index] = curr->next;
            }
            
            free(curr->value);
            free(curr->key);
            free(curr);
            map->size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

size_t map_size(const Map* map) {
    return map ? map->size : 0;
}

void map_clear(Map* map) {
    if (!map) return;

    for (size_t i = 0; i < map->capacity; i++) {
        Map_Entry* entry = map->buckets[i];
        while (entry) {
            Map_Entry* next = entry->next;
            free(entry->value);
            free(entry->key);
            free(entry);
            entry = next;
        }
        map->buckets[i] = NULL;
    }
    map->size = 0;
}

void map_free(Map* map) {
    if (!map) return;
    map_clear(map);
    free(map->buckets);
    free(map);
}

bool map_contains(const Map* map, const char* key) {
    return map_get(map, key) != NULL;
}
