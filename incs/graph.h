/**
 * @file graph.h
 * @brief Cabecera para graph.c
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "users.h"

#define MAX_USERS 200 // maxima cantidad de users en la red

typedef struct _user _User;
typedef _User *User;
typedef _User *PtrToUser;
typedef struct _globalInterests GlobalInterests;

typedef struct _edge *Edge;
typedef PtrToUser GraphList;
typedef struct _graph *Graph;

struct _edge
{
    User dest;     // usuario al que esta conectado
    double weight; // peso de la conexion
    Edge next;     // posición siguiente
};

struct _graph
{
    GraphList graphUsersList;
    int usersNumber;
};



// funciones de edges
Edge init_empty_edge(void);
Edge search_user_in_edge(Edge edge, User user);
Edge search_previous_in_edge(Edge edge, User user);
void add_edge(User user1, User user2, GlobalInterests globalInterests);
void remove_edge(User user1, User user2);
void free_all_edges(User user);
void BFS(Graph graph, User startUser);
void DFS(Graph graph, User startUser);

// funciones de grafos
Graph initialize_graph(void);
void add_user_to_graph(Graph graph, User user);
void remove_user_from_graph(Graph graph, User user);
void dijkstra(Graph graph, User source);
int dijkstra_table_index(Graph graph, User source);
void free_graph(Graph graph);

// Funciones de afinidad
double calculate_affinity(Graph graph, int user1_id, int user2_id);

#endif
