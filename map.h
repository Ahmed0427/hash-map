#ifndef _MAP_H
#define _MAP_H

typedef struct Map_Entry {
    char* key;
    void* value;
    size_t hash;
    struct Map_Entry* next;
} Map_Entry;

typedef struct {
    size_t size;
    size_t capacity;
    Map_Entry** buckets;
} Map;

void map_init(Map* map);
bool map_insert(Map* map, char* key, void* value, size_t value_size);
bool map_erase(Map* map, char* key);
void* map_get(Map* map, char* key);
size_t map_size(Map* map);
void map_free(Map* map);

#endif
