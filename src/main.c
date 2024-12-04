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
    load_users_from_file(table, "users_data.txt", graph); 

    User user1 = create_new_user("pichuldondsfd","asdasd","caca con crema",table, graph);
    User user2 = create_new_user("cacay","asdas","sdasd",table, graph);
    User user3 = create_new_user("john","asdasd","John caca",table, graph);
    User user4 = create_new_user("jofdsdfhn","asd","John caca",table, graph);
    
    add_edge(user1, user2, 1);
    add_edge(user1, user3, 1);
    add_edge(user2, user1, 1);
    add_edge(user3, user1, 1);
    add_edge(user4, user1, 1);
    add_edge(user2, user4, 1);

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


    
    save_users_to_file(table, "users_data.txt");
    
    return 0;
}
