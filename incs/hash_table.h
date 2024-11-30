/**
 * @file hash_table.h
 * @brief Cabecera para hash_table.c
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Tamaño predeterminado de la tabla hash */
#define HASH_TABLE_SIZE 100

/**
 * @struct hashnode
 * @brief Nodo individual en la tabla hash
 * @note Almacena un par clave-valor
 */
typedef struct hashnode {
    char *key; /*!< Clave única asociada al dato */
    void *data; /*!< Puntero genérico a los datos */
    struct hashnode *next; /*!< Puntero al siguiente nodo (en caso de colisión) */
} hashnode;

/**
 * @struct hashtable
 * @brief Estructura principal de la tabla hash
 */
typedef struct hashtable {
    hashnode *buckets[HASH_TABLE_SIZE]; /*!< Array de nodos (buckets) */
} hashtable;

/* Funciones principales para gestionar tablas hash */
hashtable *create_hash_table();
unsigned int hash_function(const char *key);
int insert_into_hash_table(hashtable *table, const char *key, void *data);
void *search_in_hash_table(hashtable *table, const char *key);
int delete_from_hash_table(hashtable *table, const char *key);
void free_hash_table(hashtable *table);

#endif
