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
#include "utilities.h"
#include "database.h" 

/**
 * @brief Función que ejecuta el flujo principal del programa
 * 
 * @param argc Número de argumentos
 * @param argv Argumentos ingresados al programa
 * @return int 
 */
int main(int argc, char *argv[]){
    PtrToHashTable table = NULL;
    Graph graph = NULL;
    GlobalInterests globalInterestsTable;
    User currentUser = NULL;
    heap feed;

    // obtener parámetros ingresados por el terminal
    int option=get_option(argc, argv);
    
    // si no se llama a ayuda o hay error, se inician las estructuras de datos
    if(option > 0 && option != 2){
        table = create_hash_table();
        graph = initialize_graph();
        globalInterestsTable = init_global_interests();
        // cargar base de datos si es que existe
        if (database_exists_and_not_empty() && option != 3) {
            load_database(table, graph, globalInterestsTable);
        }
        else {
            if(option==3){
                int quantity;
                if(sscanf(argv[2], "%d", &quantity)!=1){
                    printf("ERROR: No se pudo leer la cantidad de usuarios a generar\n");
                    free_hash_table(table);
                    free_graph(graph);
                    free_global_interests(globalInterestsTable);
                    exit(EXIT_FAILURE);
                }
                generate_users(quantity, table, graph, globalInterestsTable);
                generate_random_connections(graph, globalInterestsTable);
                save_all_users(graph, globalInterestsTable);
                free_all_users(table, graph);
            }
            else{
                printf("No se ha encontrado una base de datos. Ejecute './devgraph -g <cantidad de usuarios>' para generar una.\n");
                free_hash_table(table);
                free_graph(graph);
                free_global_interests(globalInterestsTable);
                exit(EXIT_FAILURE);
            }
        }
    }

    // verificación para comandos que requieren una sesión iniciada
    if(option==5||option==6||option==9||option==11||option==13||option==14||option==15){
        currentUser = current_session(table);
        if(!currentUser){
            printf("ERROR: No se ha iniciado sesión. Ejecute './devgraph -l' para iniciar sesión.\n");
            free_graph(graph);
            free_global_interests(globalInterestsTable);
            exit(EXIT_FAILURE);
        }
    }

    /* COMANDOS DEL PROGRAMA */
    switch (option){

    case 1: /* INICIO DE SESIÓN */
        login(table);
        break;

    case 2: /* CIERRA SESIÓN */
        logout();
        break;
    
    case 4: /* REGISTRAR USUARIO */
        register_user(table, graph, globalInterestsTable);
        break;

    case 5: /* PUBLICAR PUBLICACIÓN */
        write_post(currentUser, globalInterestsTable);
        break;

    case 6: /* VER PERFIL DEL USUARIO */
        print_user(currentUser, globalInterestsTable);
        break;

    case 7: {/* VER USUARIO */
        User user = search_user(argv[2], table);
        if(!user){
            printf("ERROR: Usuario no encontrado\n");
            return 0;
        }
        print_logo();
        print_user(user, globalInterestsTable);
        break;
    }
    case 8: /* VER TODOS LOS USUARIOS */
        print_logo();
        print_all_users(graph);
        break;
    
    case 9: /* SEGUIR A UN USUARIO */
        follow(currentUser, argv[2], globalInterestsTable, table);
        break;
    
    case 10: /* DEJAR DE SEGUIR A UN USUARIO */
        unfollow(currentUser, argv[2], globalInterestsTable, table);
        break;
    
    case 11: /* BORRAR CUENTA */
        delete_account(currentUser);
        break;

    case 12: /* BORRAR BASE DE DATOS */
        remove("current.dat");
        clear_database();
        break;
    
    case 13: /* EDITAR INFORMACIÓN DEL USUARIO */
        print_logo();
        edit_account(currentUser, globalInterestsTable, table);
        break;
    
    case 14: /* MOSTRAR POSTS */
        print_logo();
        search_posts(&feed, table);
        watch_posts(&feed);
        free_heap(&feed);
        break;
    
    case 15: /* MOSTRAR USUARIOS RECOMENDADOS*/
        print_logo();
        search_new_possible_friends(&feed, table, globalInterestsTable, currentUser);
        watch_suggestions(&feed);
        free_heap(&feed);
        //dijkstra(graph, currentUser);
        //BFS(graph, currentUser);
        break;
    
    case 16: /* MOSTRAR TEMAS */
        printf("Tópicos de DevGraph:\n");
        print_global_interests(globalInterestsTable);
        break;

    default:
        return 0;
        break;
    }

    // LIBERAR MEMORIA
    if(option > 0 && option != 2){
        free_all_users(table, graph);
        free_graph(graph);
        free_global_interests(globalInterestsTable);
        free_hash_table(table);
    }

    return 0;
}
