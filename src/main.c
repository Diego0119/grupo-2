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

    generate_users(80, table, graph, globalInterestsTable);

    User user1 = create_new_user("pefwefwfe", "asdasd", "fefwefwef", table, graph, globalInterestsTable);
    generate_random_connections(graph, globalInterestsTable);

    printf("Tabla de amigabilidad:\n");
    print_friendliness_table(graph);

    /*FREES*/
    free_all_users(table, graph, globalInterestsTable);
    free_graph(graph);
    free_global_interests(globalInterestsTable);
    /*----*/

    return 0;
}
