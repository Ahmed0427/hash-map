#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include <stdbool.h>

typedef struct Map Map;  // Opaque type declaration

// Initialize a new map
Map* map_create(void);

// Insert or update a key-value pair
// Returns false if allocation fails or parameters are invalid
bool map_insert(Map* map, const char* key, const void* value, size_t value_size);

// Retrieve a value by key
// Returns NULL if key not found or parameters are invalid
void* map_get(const Map* map, const char* key);

// Remove a key-value pair
// Returns false if key not found or parameters are invalid
bool map_erase(Map* map, const char* key);

// Get the number of key-value pairs in the map
size_t map_size(const Map* map);

// Check if a key exists in the map
bool map_contains(const Map* map, const char* key);

// Remove all key-value pairs from the map
void map_clear(Map* map);

// Free all resources used by the map
void map_free(Map* map);

#endif // MAP_H
