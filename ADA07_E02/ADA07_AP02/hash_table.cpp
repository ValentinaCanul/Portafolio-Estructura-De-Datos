#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Hash function: Simple sum of ASCII values modulo table size
int hash_function(const char *key, int size) {
    unsigned long hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % size;
}

// Create a new hash table
HashTable* create_hash_table(int size) {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->table = (Node **)calloc(size, sizeof(Node *));
    return ht;
}

// Insert a key-value pair into the hash table
bool insert(HashTable *ht, const char *key, IdentifierAttributes attrs) {
    int index = hash_function(key, ht->size);
    Node *current = ht->table[index];

    // Check for duplicate keys in the same scope
    while (current) {
        if (strcmp(current->key, key) == 0 && strcmp(current->attrs.scope, attrs.scope) == 0) {
            return false; // Duplicate key in the same scope
        }
        current = current->next;
    }

    // Create a new node
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->attrs.type = strdup(attrs.type);
    new_node->attrs.value = strdup(attrs.value);
    new_node->attrs.scope = strdup(attrs.scope);
    new_node->next = NULL;
    // Determine collisions: if caller provided a value (>=0), use it; otherwise
    // compute based on current bucket size (number of existing nodes)
    int existing = 0;
    Node *probe = ht->table[index];
    while (probe) { existing++; probe = probe->next; }
    if (attrs.collisions >= 0) {
        new_node->attrs.collisions = attrs.collisions;
    } else {
        new_node->attrs.collisions = existing; // 0 if empty, 1 if one existing
    }

    // Append to the tail of the list so insertion order is preserved
    if (ht->table[index] == NULL) {
        ht->table[index] = new_node;
    } else {
        Node *tail = ht->table[index];
        while (tail->next) tail = tail->next;
        tail->next = new_node;
    }
    ht->count++;
    return true;
}

// Search for a key in the hash table
bool search(HashTable *ht, const char *key, const char *scope, IdentifierAttributes *result) {
    int index = hash_function(key, ht->size);
    Node *current = ht->table[index];

    while (current) {
        if (strcmp(current->key, key) == 0 && (!scope || strcmp(current->attrs.scope, scope) == 0)) {
            if (result) {
                result->type = strdup(current->attrs.type);
                result->value = strdup(current->attrs.value);
                result->scope = strdup(current->attrs.scope);
                result->collisions = current->attrs.collisions;
            }
            return true;
        }
        current = current->next;
    }
    return false;
}

// Delete a key from the hash table
bool erase(HashTable *ht, const char *key, const char *scope) {
    int index = hash_function(key, ht->size);
    Node *current = ht->table[index];
    Node *prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0 && strcmp(current->attrs.scope, scope) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                ht->table[index] = current->next;
            }
            free(current->key);
            free(current->attrs.type);
            free(current->attrs.value);
            free(current->attrs.scope);
            free(current);
            ht->count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// Display the hash table
void display_hash_table(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        Node *current = ht->table[i];
        if (current) {
            printf("Index %d:\n", i);
            while (current) {
                printf("  Key: %s, Type: %s, Value: %s, Scope: %s\n",
                       current->key, current->attrs.type,
                       current->attrs.value, current->attrs.scope);
                current = current->next;
            }
        }
    }
}

// Destroy the hash table and free memory
void destroy_hash_table(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        Node *current = ht->table[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->attrs.type);
            free(temp->attrs.value);
            free(temp->attrs.scope);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

// Calculate the load factor of the hash table
float load_factor(HashTable *ht) {
    return (float)ht->count / ht->size;
}