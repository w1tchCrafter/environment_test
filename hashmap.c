#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hashmap.h"

// this function implements the DJB2 algorithm
static uint32_t hash_string(const char *key, uint32_t size) {
    uint32_t hash_val = 5381; // a common initial value for this algorithm
    uint32_t c;

    while ((c = *key++)) hash_val = ((hash_val << 5) + hash_val) + c;

    // the modulo operator ensures the hash index is within the valid range of the table size
    return hash_val % size;
}

Map *new_map() {
    Map *map = malloc(sizeof(Map));
    if (!map) return NULL;

    *map = (Map) {
        .size = MAP_DEFAULT_CAP,
        .table = calloc(MAP_DEFAULT_CAP, sizeof(Node*))
    };

    if (!map->table) {
        free(map);
        return NULL;
    }

    return map;
}

void free_map(Map *map) {
    if (!map) return;

    // loop over each bucket
    for (int i = 0; i < map->size; ++i) {
        Node* current = map->table[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp->key); // free the dynamically allocated key string
            free(temp);      // free the node itself
        }
    }

    free(map->table); // free the array of pointers
    free(map);        // free the hash map structure
}

void *map_get(Map *map, char *key) {
    uint32_t index = hash_string(key, map->size);
    
    // loop over the linked list at that index
    Node* current = map->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) return current->value; 
        current = current->next;
    }
    
    // key not found in the hash map
    return NULL; 
}

void map_delete(Map *map, char *key) {
    uint32_t index = hash_string(key, map->size);
    
    Node* current = map->table[index];
    Node* prev = NULL; // keep track of the previous node
    
    // loop over the linked list
    while (current != NULL) {
        // Check if the current node's key matches the one we want to delete.
        if (strcmp(current->key, key) == 0) {
            // found the node to delete
            
            if (prev == NULL) {
                // the node to delete is the head of the list
                map->table[index] = current->next;
            } else {
                // the node is in the middle or end of the list
                prev->next = current->next;
            }
            
            // free the memory for the key string and the node itself
            free(current->key);
            free(current);
            return;
        }
        
        // move to the next node
        prev = current;
        current = current->next;
    }
}

void map_insert(Map *map, char *key, void *value) {
    uint32_t index = hash_string(key, map->size);

    // check if the key already exists, if so, just update the value
    Node* current = map->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value; // update the value and return
            return;
        }
        current = current->next;
    }
    
    // key does not exist, so create a new node
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node");
        return;
    }
    
    // allocate memory for the key and copy the string
    new_node->key = strdup(key); 
    if (new_node->key == NULL) {
        perror("Failed to duplicate key string");
        free(new_node);
        return;
    }
    
    new_node->value = value;
    
    // insert the new node at the beginning of the linked list at the calculated index
    new_node->next = map->table[index];
    map->table[index] = new_node;
}