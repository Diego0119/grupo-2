#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"
#include "heaps.h"
#include "graph.h"

/**
 * @brief programa principal que demuestra la funcionalidad de usuarios, publicaciones, colas de prioridad y afinidades.
 * 
 * este programa utiliza las estructuras de usuarios, publicaciones y grafos para gestionar afinidades
 * y relaciones entre usuarios en un entorno simulado.
 * 
 * @param argc numero de argumentos pasados por linea de comandos.
 * @param argv argumentos pasados por linea de comandos. se espera:
 * - argv[1]: nombre de usuario.
 * - argv[2]: contrasena del usuario.
 * - argv[3]: nombre completo del usuario.
 * 
 * @return int codigo de retorno del programa. retorna 0 si se ejecuta correctamente, o un codigo de error.
 */
int main(int argc, char *argv[]) {
    // verificacion de los argumentos necesarios
    if (argc < 4) {
        printf("uso: %s <username> <password> <name>\n", argv[0]);
        return EXIT_FAILURE; // salir si no se proporcionan los argumentos necesarios
    }

    printf("\n=== creacion de usuario y publicaciones ===\n");

    // crear un nuevo usuario utilizando los datos proporcionados por linea de comandos
    User user = create_new_user(argv[1], argv[2], argv[3]);

    // insertar publicaciones al usuario
    insert_post(user.posts, "Hola mundooo"); // primera publicacion
    insert_post(user.posts, "chill de cojones"); // segunda publicacion

    // imprimir informacion completa del usuario, incluyendo publicaciones
    print_user(user);

    // liberar recursos asociados al usuario y su lista de publicaciones
    delete_user(&user);

    printf("\n=== publicaciones con colas de prioridad ===\n");

    // crear un heap vacio para gestionar publicaciones
    heap h = {0}; // inicializacion del heap

    // insertar publicaciones al heap con un nivel de prioridad
    insert_new_post(&h, "pepito123", 50, "hoy comi pancito, amo la cocina!");   // publicacion con prioridad 50
    insert_new_post(&h, "masterjoy", 75, "soy tan suertudo, amo apostar");      // publicacion con prioridad 75
    insert_new_post(&h, "jotabond12", 95, "que mala suerte, como odio los dias lunes"); // publicacion con prioridad 95
    insert_new_post(&h, "userDesk", 65, "a veces me gusta correr");            // publicacion con prioridad 65
    insert_new_post(&h, "Charmy404", 85, "amo jugar a la consola");            // publicacion con prioridad 85

    // imprimir todas las publicaciones en el heap
    printf("\npublicaciones en el heap:\n");
    print_heap(&h); // funcion que muestra todas las publicaciones actuales en el heap

    // ver las publicaciones en orden de prioridad
    printf("\nver publicaciones (prioridad decreciente):\n");
    ver_posts(&h); // extrae y muestra publicaciones del heap en orden de prioridad

    // verificar el estado del heap tras la extraccion de publicaciones
    printf("\nestado del heap despues de ver publicaciones:\n");
    print_heap(&h); // muestra el estado actual del heap

    printf("\n=== gestion de amistades y afinidades (grafos) ===\n");

    // crear un grafo para gestionar usuarios y sus amistades
    Graph graph;
    initialize_graph(&graph);

    // agregar usuarios al grafo
    int user1 = add_user(&graph, "Alice");
    int user2 = add_user(&graph, "Bob");
    int user3 = add_user(&graph, "Charlie");
    int user4 = add_user(&graph, "Diana");

    // crear amistades con pesos que representen la afinidad
    add_friendship(&graph, user1, user2, 50); // Alice y Bob con afinidad 50
    add_friendship(&graph, user1, user3, 75); // Alice y Charlie con afinidad 75
    add_friendship(&graph, user2, user4, 90); // Bob y Diana con afinidad 90
    add_friendship(&graph, user3, user4, 60); // Charlie y Diana con afinidad 60

    // mostrar las amistades de cada usuario
    printf("\namistades de Alice:\n");
    display_friends(&graph, user1);

    printf("\namistades de Bob:\n");
    display_friends(&graph, user2);

    // eliminar una amistad
    printf("\neliminando la amistad entre Alice y Charlie...\n");
    remove_friendship(&graph, user1, user3);
    printf("amistades de Alice despues de eliminar a Charlie:\n");
    display_friends(&graph, user1);

    // mostrar las distancias (afinidades) desde un usuario con Dijkstra
    printf("\ncalculo de afinidades desde Alice:\n");
    dijkstra(&graph, user1);

    return EXIT_SUCCESS;
}
