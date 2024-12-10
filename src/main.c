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
#include "utilities.h"
#include "database.h" 

int main(void)
{
    PtrToHashTable table = create_hash_table();
    Graph graph = initialize_graph();
    GlobalInterests globalInterestsTable = init_global_interests();

    int loadedFromDB = 0;

    if (database_exists_and_not_empty()) {
        printf("Se ha detectado una base de datos existente. ¿Deseas cargarla?\n");
        printf("1. Sí, cargar usuarios existentes.\n");
        printf("2. No, ignorar y generar usuarios nuevos.\n");
        int opcion;
        if (scanf("%d",&opcion)!=1) opcion=1;
        if (opcion == 1) {
            load_all_users(table, graph, globalInterestsTable);
            loadedFromDB = 1;
        } else {
    generate_users(80, table, graph, globalInterestsTable);
    generate_users(80, table, graph, globalInterestsTable);

            generate_users(80, table, graph, globalInterestsTable);

            User user1 = create_new_user("pefwefwfe", "asdasd", "fefwefwef", table, graph, globalInterestsTable);
            generate_random_connections(graph, globalInterestsTable);
            print_user(graph->graphUsersList->next->next, globalInterestsTable);
            dijkstra(graph, user1);
        }
    } else {
        generate_users(80, table, graph, globalInterestsTable);
        User user1 = create_new_user("pefwefwfe", "asdasd", "fefwefwef", table, graph, globalInterestsTable);
        generate_random_connections(graph, globalInterestsTable);
        print_user(graph->graphUsersList->next->next, globalInterestsTable);
        dijkstra(graph, user1);
    }

    //printf("Tabla de amigabilidad:\n");
    //print_friendliness_table(graph);

    

    if (!loadedFromDB) {
        save_all_users(graph, globalInterestsTable);
    }

    confirm_and_cleanup();

    free_all_users(table, graph, globalInterestsTable);
    free_graph(graph);
    free_global_interests(globalInterestsTable);

    return 0;
}
