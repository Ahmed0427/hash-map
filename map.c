#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "map.h"

#define INIT_CAPACITY 16

void map_init(Map* map) {
    map->size = 0;
    map->capacity = INIT_CAPACITY;
    map->buckets = calloc(map->capacity, sizeof(Map_Entry*));
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static size_t get_hash(const char* key) {
    size_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (size_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

static bool map_extend(Map* map) {
    size_t new_capacity = map->capacity * 2;
    Map_Entry** new_buckets = calloc(new_capacity, sizeof(Map_Entry*));

    if (!new_buckets) return false;
    
    for (int i = 0; i < (int)map->capacity; i++) {
        Map_Entry* entry = map->buckets[i];

        while (entry) {
            Map_Entry* next = entry->next;
            int new_index = entry->hash % new_capacity;
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

void* void_dup(void* ptr, size_t size) {
    if (!ptr || !size) return NULL;
    void* new_ptr = malloc(size); 
    if (!new_ptr) return NULL;
    memcpy(new_ptr, ptr, size);
    return new_ptr;
}

bool map_insert(Map* map, char* key, void* value, size_t value_size) {
    if (!value || !value_size || !map || !key) return false;

    if (map->size >= map->capacity * 0.75) {
        if(!map_extend(map)) return false;
    }

    size_t hash = get_hash(key);
    size_t index = hash % map->capacity;

    Map_Entry* t_entry = map->buckets[index];

    while (t_entry) {
        if (t_entry->hash == hash && strcmp(t_entry->key, key) == 0) {
            free(t_entry->value);
            t_entry->value = void_dup(value, value_size);
            return t_entry->value != NULL;
        }
        t_entry = t_entry->next;
    }

    Map_Entry* new_entry = malloc(sizeof(Map_Entry));

    if (!new_entry) return false;

    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;

    new_entry->key = malloc(strlen(key) + 1);
    if (!new_entry->key) return false;
    memcpy(new_entry->key, key, strlen(key) + 1);

    new_entry->value = void_dup(value, value_size);
    if (!new_entry->value) return false;

    new_entry->hash = hash;
    map->size++;

    return true;
}

void* map_get(Map* map, char* key) {
    if (!key || !map) return NULL;

    size_t hash = get_hash(key);
    size_t index = hash % map->capacity;

    Map_Entry* t_entry = map->buckets[index];

    while (t_entry) {
        if (t_entry->hash == hash && strcmp(t_entry->key, key) == 0) {
            return t_entry->value;
        }
        t_entry = t_entry->next;
    }

    return NULL;
}

bool map_erase(Map* map, char* key) {
    if (!key || !map) return false;

    size_t hash = get_hash(key);
    size_t index = hash % map->capacity;

    Map_Entry* prev_entry = NULL;
    Map_Entry* curr_entry = map->buckets[index];

    while (curr_entry) {
        if (curr_entry->hash == hash && strcmp(curr_entry->key, key) == 0) {
            break;
        }
        prev_entry = curr_entry;
        curr_entry = curr_entry->next;
    }

    if (curr_entry == NULL) return false;

    if (prev_entry == NULL) {
        map->buckets[index] = curr_entry->next;
        curr_entry->next = NULL;
        free(curr_entry->value);
        free(curr_entry->key);
        free(curr_entry);
    }
    else {
        prev_entry->next = curr_entry->next;
        curr_entry->next = NULL;
        free(curr_entry->value);
        free(curr_entry->key);
        free(curr_entry);
    }

    return true;
}

size_t map_size(Map* map) {
    return map->size;
}

void map_free(Map* map) {
    for (int i = 0; i < (int)map->capacity; i++) {
        Map_Entry* entry = map->buckets[i];

        while (entry) {
            Map_Entry* next = entry->next;

            free(entry->value);
            free(entry->key);
            free(entry);

            entry = next;
        }
    }
    free(map->buckets);
}

