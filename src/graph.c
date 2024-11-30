/**
 * @file graph.c
 * @brief Implementación de funciones para gestionar grafos y relaciones entre usuarios en una red social
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include "graph.h"

/**
 * @brief Inicializa un grafo vacío
 * 
 * @param graph Puntero al grafo que será inicializado
 * @note Se configuran las listas de adyacencia y el contador de usuarios en 0.
 */
void initialize_graph(Graph *graph)
{
    graph->users_number = 0;
    for (int i = 0; i < MAX_USERS; i++)
    {
        graph->adyacent_friendship_list[i] = NULL;
        graph->friends_count[i] = 0;
    }
}

/**
 * @brief Añade un nuevo usuario al grafo
 * 
 * @param graph Puntero al grafo
 * @param name Nombre del usuario a añadir
 * @return int Devuelve el ID del usuario añadido o -1 si falla
 * @note Si se alcanza el número máximo de usuarios, no se realiza la inserción.
 */
int add_user(Graph *graph, const char *name)
{
    if (graph->users_number >= MAX_USERS)
    {
        printf("Número máximo de usuarios alcanzado\n");
        return -1;
    }

    User new_user;
    strcpy(new_user.name, name);
    graph->users[graph->users_number] = new_user;

    return graph->users_number++;
}

/**
 * @brief Establece una amistad entre dos usuarios
 * 
 * @param graph Puntero al grafo
 * @param user1 ID del primer usuario
 * @param user2 ID del segundo usuario
 * @param weight Nivel de afinidad entre los usuarios (peso de la relación)
 * @note Esta función es simétrica: añade la conexión en ambas direcciones.
 */
void add_friendship(Graph *graph, int user1, int user2, int weight)
{
    if (user1 == user2)
    {
        printf("Un usuario no puede ser amigo de sí mismo\n");
        return;
    }

    // Obtiene la lista de amigos del usuario 1
    Edge *friends_of_user1 = graph->adyacent_friendship_list[user1];
    // Número actual de amigos del usuario 1
    int current_size1 = graph->friends_count[user1];
    // Redimensiona el arreglo dinámico para añadir un nuevo amigo
    friends_of_user1 = realloc(friends_of_user1, sizeof(Edge) * (current_size1 + 1));
    // Añade el usuario 2 como amigo del usuario 1
    friends_of_user1[current_size1].dest = user2;
    // Asigna el peso de la relación (nivel de afinidad)
    friends_of_user1[current_size1].weight = weight;
    // Actualiza la lista de adyacencia
    graph->adyacent_friendship_list[user1] = friends_of_user1;
    graph->friends_count[user1]++;

    // Repite los mismos pasos para añadir al usuario 1 como amigo del usuario 2
    Edge *friends_of_user2 = graph->adyacent_friendship_list[user2];
    int current_size2 = graph->friends_count[user2];
    friends_of_user2 = realloc(friends_of_user2, sizeof(Edge) * (current_size2 + 1));
    friends_of_user2[current_size2].dest = user1;
    friends_of_user2[current_size2].weight = weight;
    graph->adyacent_friendship_list[user2] = friends_of_user2;
    graph->friends_count[user2]++;
}

/**
 * @brief Muestra los amigos de un usuario
 * 
 * @param graph Puntero al grafo
 * @param user_id ID del usuario
 * @note Imprime los IDs y pesos de las conexiones de cada amigo del usuario.
 */
void display_friends(Graph *graph, int user_id)
{
    if (user_id < 0 || user_id >= graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }

    printf("Amigos del usuario con ID %d:\n", user_id);

    // Obtiene la lista de amigos del usuario
    Edge *friends_of_user = graph->adyacent_friendship_list[user_id];
    int friends_count = graph->friends_count[user_id];

    // Recorre e imprime cada amigo
    for (int i = 0; i < friends_count; i++)
    {
        printf("Amigo ID: %d, Peso de la conexión: %d\n", friends_of_user[i].dest, friends_of_user[i].weight);
    }

    if (friends_count == 0)
    {
        printf("El usuario con ID %d no tiene amigos\n", user_id);
    }
}

/**
 * @brief Elimina una relación de amistad entre dos usuarios
 * 
 * @param graph Puntero al grafo
 * @param user1_id ID del primer usuario
 * @param user2_id ID del segundo usuario
 * @note Se elimina la relación de ambas listas de adyacencia.
 */
void remove_friendship(Graph *graph, int user1_id, int user2_id)
{
    if (user1_id < 0 || user1_id >= graph->users_number || user2_id < 0 || user2_id >= graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }

    // Elimina la relación del usuario 1 hacia el usuario 2
    Edge *current_friendship_list = graph->adyacent_friendship_list[user1_id];
    int friends_count = graph->friends_count[user1_id];

    for (int i = 0; i < friends_count; i++)
    {
        if (current_friendship_list[i].dest == user2_id)
        {
            current_friendship_list[i] = current_friendship_list[friends_count - 1];
            current_friendship_list = realloc(current_friendship_list, sizeof(Edge) * (friends_count - 1));
            graph->adyacent_friendship_list[user1_id] = current_friendship_list;
            graph->friends_count[user1_id]--;
            break;
        }
    }

    // Elimina la relación del usuario 2 hacia el usuario 1
    current_friendship_list = graph->adyacent_friendship_list[user2_id];
    friends_count = graph->friends_count[user2_id];

    for (int i = 0; i < friends_count; i++)
    {
        if (current_friendship_list[i].dest == user1_id)
        {
            current_friendship_list[i] = current_friendship_list[friends_count - 1];
            current_friendship_list = realloc(current_friendship_list, sizeof(Edge) * (friends_count - 1));
            graph->adyacent_friendship_list[user2_id] = current_friendship_list;
            graph->friends_count[user2_id]--;
            break;
        }
    }
}

/**
 * @brief Utiliza el algoritmo de Dijkstra para calcular distancias mínimas
 * 
 * @param graph Puntero al grafo
 * @param source ID del usuario origen
 * @note Imprime las distancias mínimas desde el nodo origen a todos los demás nodos.
 */
void dijkstra(Graph *graph, int source)
{
    int users_number = graph->users_number;
    int *visited = malloc(sizeof(int) * users_number);
    int *distance = malloc(sizeof(int) * users_number);

    for (int i = 0; i < users_number; i++)
    {
        distance[i] = INT_MAX;
        visited[i] = 0;
    }

    distance[source] = 0;

    for (int count = 0; count < users_number - 1; count++)
    {
        int min_distance = INT_MAX, current_node = -1;

        for (int i = 0; i < users_number; i++)
        {
            if (!visited[i] && distance[i] < min_distance)
            {
                min_distance = distance[i];
                current_node = i;
            }
        }

        if (current_node == -1)
            break;

        visited[current_node] = 1;

        Edge *neighbors = graph->adyacent_friendship_list[current_node];
        int friends_count = graph->friends_count[current_node];

        for (int i = 0; i < friends_count; i++)
        {
            int neighbor = neighbors[i].dest;
            int weight = neighbors[i].weight;

            if (!visited[neighbor] && distance[current_node] + weight < distance[neighbor])
            {
                distance[neighbor] = distance[current_node] + weight;
            }
        }
    }

    printf("Distancias desde el nodo %d:\n", source);
    for (int i = 0; i < users_number; i++)
    {
        if (distance[i] == INT_MAX)
            printf("Usuario %d: Inalcanzable\n", i);
        else
            printf("Usuario %d: %d\n", i, distance[i]);
    }

    free(distance);
    free(visited);
}
