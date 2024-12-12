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

typedef struct _user _User;
typedef _User *User;
typedef _User *PtrToUser;
typedef struct _globalInterests GlobalInterests;

typedef struct _edge *Edge;
typedef PtrToUser GraphList;
typedef struct _graph *Graph;

/**
 * @struct _edge 
 * @brief Estructura que representa una lista de arista de un usuario perteneciente al grafo
 */
struct _edge{
    User dest; /*!< Usuario destino */
    double weight; /*!< Peso de la arista */
    Edge next; /*!< Siguiente en la lista de aristas */
};

/**
 * @struct _graph
 * @brief Estructura que representa un grafo
 */
struct _graph{
    GraphList graphUsersList; /*!< Lista de usuarios en el grafo */
    int usersNumber; /*!< Numero de usuarios en el grafo */
};

/* FUNCIONES DE EDGE */
Edge init_empty_edge(void);
Edge search_user_in_edge(Edge edge, User user);
Edge search_previous_in_edge(Edge edge, User user);
void add_edge(User user1, User user2, GlobalInterests globalInterests);
void remove_edge(User user1, User user2);
void free_all_edges(User user);
void BFS(Graph graph, User startUser);
void DFS(Graph graph, User startUser);

/* FUNCIONES DE GRAPH*/
Graph initialize_graph(void);
void add_user_to_graph(Graph graph, User user);
void remove_user_from_graph(Graph graph, User user);
void dijkstra(Graph graph, User source);
int dijkstra_table_index(Graph graph, User source);
void free_graph(Graph graph);

/* FUNCIONES DE AFINIDAD*/
double calculate_affinity(Graph graph, int user1_id, int user2_id);

#endif
