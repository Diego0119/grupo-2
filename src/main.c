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

    printf("Generando usuarios aleatorios...\n");
    User user1 = create_new_user("Admin", "pass", "DIOS", table, graph);
    generate_users(50, table, graph);
    int users_quantity = graph->usersNumber;
    printf("Users quantity %d\n", users_quantity);

    generate_random_connections(users_quantity, graph);

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
