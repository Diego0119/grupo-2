#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASH_TABLE_SIZE 100

typedef struct post {
    char *content;
    struct post *next;
} post;

typedef struct user {
    char *username;
    int popularity;
    post *posts;
} user;

typedef struct hashnode {
    char *key;
    void *data;
    struct hashnode *next;
} hashnode;

typedef struct hashtable {
    hashnode *buckets[HASH_TABLE_SIZE];
} hashtable;

hashtable *create_hash_table();
unsigned int hash_function(const char *key);
int insert_into_hash_table(hashtable *table, const char *key, void *data);
void *search_in_hash_table(hashtable *table, const char *key);
int delete_from_hash_table(hashtable *table, const char *key);
void free_hash_table(hashtable *table);

user *create_user(const char *username);
void add_post(user *user, const char *content);
void display_posts(user *user);

#endif
