
# Hash Map

This is a simple hash map implementation in C. It provides basic functionality like inserting, retrieving, deleting elements, and getting the size of the map. The map uses separate chaining for collision resolution.

## Functions

### `void map_init(Map* map)`
Initializes the map with default capacity.

- **Parameters:**
    - `map`: Pointer to a Map structure to be initialized.

### `bool map_insert(Map* map, char* key, void* value, size_t value_size)`
Inserts a key-value pair into the map. If the key already exists, the value is updated.

- **Parameters:**
    - `map`: Pointer to the Map.
    - `key`: The key of the entry.
    - `value`: The value to store.
    - `value_size`: Size of the value being stored.
- **Returns:**
    - `true` if insertion was successful.
    - `false` if there was an error.

### `bool map_erase(Map* map, char* key)`
Erases an entry with the specified key from the map.

- **Parameters:**
    - `map`: Pointer to the Map.
    - `key`: The key of the entry to delete.
- **Returns:**
    - `true` if the key was erased.
    - `false` if the key was not found.

### `void* map_get(Map* map, char* key)`
Retrieves the value associated with a given key.

- **Parameters:**
    - `map`: Pointer to the Map.
    - `key`: The key of the entry to retrieve.
- **Returns:**
    - Pointer to the value associated with the key.
    - `NULL` if the key is not found.

### `size_t map_size(Map* map)`
Returns the number of entries currently in the map.

- **Parameters:**
    - `map`: Pointer to the Map.
- **Returns:**
    - The number of key-value pairs in the map.

### `void map_free(Map* map)`
Frees all memory associated with the map, including the map itself and its entries.

- **Parameters:**
    - `map`: Pointer to the Map.

## Compilation

To compile the test suite and map implementation:

```bash
gcc -o test test.c map.c
./test

or just

make run
