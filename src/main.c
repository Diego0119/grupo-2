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

int main()
{
    PtrToHashTable table = create_hash_table();
    Graph graph = initialize_graph();

    User user = create_new_user("pichuldondsfd","asdasd","caca con crema",table, graph);
    User user2 = create_new_user("cacay","asdas","sdasd",table, graph);
    User user3 = create_new_user("john","asdasd","John caca",table, graph);
    User user4 = create_new_user("jofdsdfhn","asd","John caca",table, graph);
    
    add_edge(user, user2, 1);
    add_edge(user, user3, 1);
    add_edge(user2, user, 1);
    add_edge(user3, user, 1);
    add_edge(user4, user, 1);
    add_edge(user2, user4, 1);
    
    print_user(user);

    print_all_users(graph);

    dijkstra(graph, user);
    /* frees */
    free_all_users(table, graph);
    free_graph(graph);



    

    return 0;
}