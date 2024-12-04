/**
 * @file hash_table.c
 * @brief Implementación de funciones para gestionar tablas hash, usuarios y publicaciones
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */
#include "hash_table.h"

/**
 * @brief Crea una nueva tabla hash vacía
 * 
 * @return HashTable* Puntero a la nueva tabla hash
 */
HashTable* create_hash_table(void) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    if (!table) {
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

/**
 * @brief Calcula el índice de la tabla hash para una clave
 * 
 * @param key Clave para calcular el hash
 * @return unsigned int Índice calculado
 */
unsigned int hash_function(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % HASH_TABLE_SIZE;
}

/**
 * @brief Inserta un nuevo elemento en la tabla hash
 * 
 * @param table Tabla hash
 * @param key Clave del elemento
 * @param data Puntero a los datos
 * @return int 0 si se inserta correctamente, -1 si hay un error
 */
int insert_into_hash_table(HashTable *table, char *key, void *data) {
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
 * @brief Busca un elemento en la tabla hash por su clave
 * 
 * @param table Tabla hash
 * @param key Clave del elemento a buscar
 * @return void* Puntero a los datos encontrados o NULL si no existe
 */
void *search_in_hash_table(HashTable *table, char *key) {
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
 * @brief Elimina un elemento de la tabla hash
 * 
 * @param table Tabla hash
 * @param key Clave del elemento a eliminar
 * @return int 0 si se elimina correctamente, -1 si no se encuentra
 */
int delete_from_hash_table(HashTable *table, char *key) {
    if (!table || !key) return -1;
    unsigned int index = hash_function(key);
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
 * @brief Libera toda la memoria utilizada por la tabla hash
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
