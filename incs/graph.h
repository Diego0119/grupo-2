/**
 * @file graph.h
 * @author Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 * @brief Cabecera para graph.c
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h> // para poder usar INT_MAX como infinito

#include "users.h"

#define MAX_USERS 100 // máxima cantidad de usuarios en la red

// Estructura de conexión entre usuarios (arista)
typedef struct Edge
{
    int dest;   // ID del usuario al que está conectado
    int weight; // Peso de la conexión
} Edge;

// Representación del grafo
typedef struct Graph
{
    int users_number;                       // Número actual de usuarios
    User users[MAX_USERS];                  // Lista de usuarios
    Edge *adyacent_friendship_list[MAX_USERS]; // Lista de adyacencia
    int friends_count[MAX_USERS];           // Contador de amigos de un usuario
} Graph;

// Funciones de los grafos
void initialize_graph(Graph *graph);
int add_user(Graph *graph, const char *name);
void add_friendship(Graph *graph, int user1, int user2, int weight);
void remove_user(Graph *graph, int user_id);
void display_friends(Graph *graph, int user_id);
void remove_friendship(Graph *graph, int user1, int user2);
void remove_all_friendships(Graph *graph, int user_id);
void remove_reference_to_user(Graph *graph, int user_id);
void dijkstra(Graph *graph, int source);

// Funciones de afinidad
double calculate_affinity(Graph *graph, int user1_id, int user2_id);

#endif
