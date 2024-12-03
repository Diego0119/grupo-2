/**
 * @file main.c
 * @brief Flujo principal del programa
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"
#include "heaps.h"
#include "graph.h"

int main(void)
{
    PtrToHashTable table = create_hash_table();
    Graph graph = initialize_graph();
    User user1 = create_new_user("Admin", "pass", "DIOS", table, graph);

    printf("Generando usuarios aleatorios...\n");

    generate_users(80, table, graph);
    generate_random_connections(540, graph);

    printf("\nProbando BFS:\n");
    BFS(graph, user1);

    printf("\nProbando DFS:\n");
    DFS(graph, user1);

    print_user(user1);

    print_all_users(graph);

    dijkstra(graph, user1);
    /* frees */
    free_all_users(table, graph);
    free_graph(graph);

    return 0;
}
