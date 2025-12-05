#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>

// Constants
#define TABLE_SIZE 101
// Reason for 101: Prime number to reduce collisions, about 78% of the total ASCII characters.
#define EMPTY -1
#define DELETED -2

// Structure for identifier attributes
typedef struct {
    char *type;    // Data type (e.g., int, float, string)
    char *value;   // Value (NULL if not assigned)
    char *scope;   // Scope (Global or Local)
    int collisions; // Number of collisions at insertion time (0 or 1)
} IdentifierAttributes;

// Node for linked list (collision handling)
typedef struct Node {
    char *key;                     // Identifier name
    IdentifierAttributes attrs;    // Attributes of the identifier
    struct Node *next;             // Pointer to the next node
} Node;

// Hash table structure
typedef struct {
    Node **table;  // Array of pointers to linked lists
    int size;      // Size of the table
    int count;     // Number of active elements
} HashTable;

// Function prototypes
int hash_function(const char *key, int size);
HashTable* create_hash_table(int size);
bool insert(HashTable *ht, const char *key, IdentifierAttributes attrs);
bool search(HashTable *ht, const char *key, const char *scope, IdentifierAttributes *result);
bool erase(HashTable *ht, const char *key, const char *scope);
void display_hash_table(HashTable *ht);
void destroy_hash_table(HashTable *ht);
float load_factor(HashTable *ht);

#endif // HASH_TABLE_H
