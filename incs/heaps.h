/**
 * @file users.h
 * @author Iván Mansilla, Franco Aguilar, Diego Sanhueza, Duvan Figueroa, Nicolás Álvarez, Miguel Maripillan
 * @brief Cabecera para users.c
 */

#ifndef HEAPS_H
#define HEAPS_H

typedef struct _post post;
typedef struct _heap heap;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEAP_SIZE 100
#define MAX_NAME 100
#define MAX_CONTENT 256

/**
 * @struct _post
 * @brief Estructura que almacena los datos de una publicación
 */
struct _post {
    char user_name[MAX_NAME];  // Nombre del usuario (único para cada usuario)
    char name[MAX_NAME];       // Nombre real
    int priority;         // Prioridad (por ejemplo, basado en intereses del usuario)
    char content[MAX_CONTENT];    // Contenido de la publicación
};

/**
 * @struct _heap
 * @brief Estructura para el heap
 */
struct _heap {
    post posts[MAX_HEAP_SIZE]; // Arreglo de publicaciones
    int size; // Este tamaño refleja el número de elementos en el heap
};

//Funciones para gestionar heaps
void heapify_up(heap* h, int index); // Función para asegurar que el heap mantenga la propiedad (heapify_up)
void heapify_down(heap* h, int index); // Función para hacer heapify_down (restaurar la propiedad después de extraer el máximo)

// funciones para gestionar publicaciones
void insert_new_post(heap* h, const char* user_name, int priority, const char* content); // Función para insertar una publicación en el heap
void extract_max(heap* h); // Función para extraer el máximo (el nodo más relevante)
void print_heap(heap* h); // Función para imprimir el heap
void ver_posts(heap* h); // Función para ver publicaciones del heap

#endif