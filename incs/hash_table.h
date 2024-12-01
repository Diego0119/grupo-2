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

#include "utilities.h"

/* Tamaño predeterminado de la tabla hash */
#define HASH_TABLE_SIZE 100


typedef struct _hashnode Hashnode;
typedef struct _hashtable HashTable;
typedef HashTable* PtrToHashTable;

/**
 * @struct hashnode
 * @brief Nodo individual en la tabla hash
 * @note Almacena un par clave-valor
 */
struct _hashnode {
    char *key; /*!< Clave única asociada al dato */
    void *data; /*!< Puntero genérico a los datos */
    Hashnode *next; /*!< Puntero al siguiente nodo (en caso de colisión) */
};

/**
 * @struct _hashtable
 * @brief Estructura principal de la tabla hash
 */
struct _hashtable {
    Hashnode *buckets[HASH_TABLE_SIZE]; /*!< Array de nodos (buckets) */
};

/* Funciones principales para gestionar tablas hash */
HashTable* create_hash_table(void);
unsigned int hash_function(const char *key);
int insert_into_hash_table(HashTable *table, char *key, void *data);
void *search_in_hash_table(HashTable *table, char *key);
int delete_from_hash_table(HashTable *table, char *key);
void free_hash_table(HashTable *table);

#endif
