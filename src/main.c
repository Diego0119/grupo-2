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
#include "hash_table.h"
#include "files.h"

int main(void)
{
    /* INITS */
    PtrToHashTable table = create_hash_table();
    Graph graph = initialize_graph();
    GlobalInterests globalInterestsTable = init_global_interests();
    /*------*/

    generate_users(100, table, graph, globalInterestsTable);

    User user1 = create_new_user("pichuldondsfd","asdasd","caca con crema",table, graph, globalInterestsTable);
    generate_random_connections(graph);
    print_all_users(graph);
    print_user(user1);
    dijkstra(graph, user1);
    
    




    /*FREES*/
    free_all_users(table, graph, globalInterestsTable);
    free_graph(graph);
    free_global_interests(globalInterestsTable);
    /*----*/

    return 0;
}
