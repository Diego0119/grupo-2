#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utilities.h"

#define HASH_TABLE_SIZE 100

typedef struct _hashnode Hashnode;
typedef struct _hashtable HashTable;
typedef HashTable* PtrToHashTable;

typedef struct _hashnode {
    char *key;
    void *data;
    struct Hashnode *next;
};

typedef struct _hashtable {
    Hashnode *buckets[HASH_TABLE_SIZE];
};


PtrToHashTable create_hash_table();
unsigned int hash_function(const char *key);
int insert_into_hash_table(HashTable *table, const char *key, void *data);
void *search_in_hash_table(HashTable *table, const char *key);
int delete_from_hash_table(HashTable *table, const char *key);
void free_hash_table(HashTable *table);

#endif
