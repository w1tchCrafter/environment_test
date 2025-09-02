#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAP_DEFAULT_CAP 100

// Each entry in the hashtable is represented as a linked list
// storing both the entry's key and value
typedef struct Node {
    // also storing the key in case of hash colision
    char *key;

    // void pointer to support multiple types
    void *value;

    // pointer to the next item on the list
    struct Node *next;
} Node;


// hashmap struct
typedef struct {
    // the size indicates the number o buckets within the map
    // it is used by the hash function to ensure the result is always within the number of buckets
    uint32_t size;

    // the table also called the buckets
    // it is a common strategy to handle hash collision by storing data into a collection instead of single entrys
    // if a two or more keys maps to the same bucket they will be stored in a collection
    // the more buckets the more unlike is to happen a hash collision
    Node **table;
} Map;

// not commenting here
// the names are self documenting :}

Map *new_map();
void free_map(Map *map);
void *map_get(Map *map, char *key);
void map_delete(Map *map, char *key);
void map_insert(Map *map, char *key, void *value);
