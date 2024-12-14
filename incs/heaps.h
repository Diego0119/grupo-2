/**
 * @file heaps.h
 * @brief Cabecera para heaps.c
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#ifndef HEAPS_H
#define HEAPS_H

typedef struct _heap heap;
typedef struct _post post;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"
#include "hash_table.h"
#include "graph.h"

#define MAX_HEAP_SIZE 1000 // recomendamos que sea una cantidad equivalente a la generación de usuarios o más

/**
 * @struct _post
 * @brief Estructura que almacena los datos de una publicación
 */
struct _post {
    char* user_name;  // Nombre del usuario (único para cada usuario)
    char* name;       // Nombre real
    double priority;  // Prioridad (por ejemplo, basado en intereses del usuario)
    char* content;    // Contenido de la publicación
};

/**
 * @struct _heap
 * @brief Estructura para el heap
 */
struct _heap {
    post posts[MAX_HEAP_SIZE]; // Arreglo de publicaciones y sugerencias, se trabaja como arreglo por que en este tipo de casos es más eficiente que un arreglo dinámico
    int size; // Este tamaño refleja el número de elementos en el heap
};


//Funciones para gestionar propiedades heaps
void heapify_up(heap* h, int index); // Función para asegurar que el heap mantenga la propiedad (heapify_up)
void heapify_down(heap* h, int index); // Función para hacer heapify_down (restaurar la propiedad después de extraer el máximo)
void extract_max(heap* h, int option); // Función para extraer el máximo (el nodo más relevante)
void extract_min(heap* h, int option); // Función para extraer el mínimo (el nodo menos relevante)
void print_heap(heap* h); // Función para imprimir el heap

// funciones para gestionar publicaciones y sugerencias
void insert_new_item(heap* h, const char* user_name, double priority, const char* content); // Función para insertar una publicación en el heap
void watch_posts(heap* h); // Función para ver publicaciones del heap
void watch_suggestions_friends_of_friends(heap* h); // Función para ver sugerencias del heap
void watch_suggestions_by_interests(heap* h); // Función para ver sugerencias del heap
void search_posts(heap* h, PtrToHashTable table); // Función para buscar publicaciones para colocar en el heap
void search_new_possible_friends(heap* h, PtrToHashTable table, GlobalInterests globalInterestsTable, User currentUser); // Función para buscar usuarios con intereses similares a los de un usuario
void free_heap(heap* h); // Función para liberar memoria del heap
void dijkstra(heap* h, Graph graph, User source);

#endif
