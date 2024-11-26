/**
 * @file hash_table.c
 * @author 
 * @brief 
 */
#include "hash_table.h"

/**
 * @brief Crear una tabla hash vacia
 * 
 * @return PtrToHashTable Tabla hash creada 
 */
PtrToHashTable create_hash_table() {
    PtrToHashTable table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) exit(EXIT_FAILURE);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

/**
 * @brief Inserta un elemento en la tabla hash
 * 
 * @param table Tabla hash
 * @param key String correspondiente
 * @param data Dato a insertar
 * @return int Índice hash correspondiente
 */
int insert_into_hash_table(HashTable *table, const char *key, void *data) {
    if (!table || !key) return -1;
    unsigned int index = jenkins_hash(key);
    Hashnode *new_node = (Hashnode *)malloc(sizeof(Hashnode));
    if (!new_node) return -1;
    new_node->key = strdup(key);
    new_node->data = data;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    return 0;
}

/**
 * @brief Buscar elemento en tabla hash
 * 
 * @param table Tabla hash
 * @param key String a buscar
 * @return void* Elemento correspondiente a la key
 */
void *search_in_hash_table(HashTable *table, const char *key) {
    if (!table || !key) return NULL;
    unsigned int index = jenkins_hash(key);
    Hashnode *current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) return current->data;
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Eliminar elemento de la tabla hash
 * 
 * @param table Tabla Hash
 * @param key String a eliminar
 * @return int 
 */
int delete_from_hash_table(HashTable *table, const char *key) {
    if (!table || !key) return -1;
    unsigned int index = jenkins_hash(key);
    Hashnode *current = table->buckets[index];
    Hashnode *previous = NULL;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }
            free(current->key);
            free(current);
            return 0;
        }
        previous = current;
        current = current->next;
    }
    return -1;
}

/**
 * @brief Eliminar tabla hash
 * 
 * @param table Tabla hash
 */
void free_hash_table(HashTable *table) {
    if (!table) return;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Hashnode *current = table->buckets[i];
        while (current) {
            Hashnode *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table);
}

