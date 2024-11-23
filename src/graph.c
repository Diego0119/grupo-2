#include "header.h"

void initialize_graph(Graph *graph)
{
    graph->users_number = 0;
    for (int i = 0; i < MAX_USERS; i++)
    {
        graph->adyacent_friendship_list[i] = NULL;
    }
}

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

// en esta funcion se debe actualizar la lista de adyacencia
// adyacent_list[ 1 | 2 | 3 | 4 | 5]
//                |   |       |
//                v   v       v
//              [2|3] NULL   [3|4]
void add_friendship(Graph *graph, int user1, int user2, int weight)
{
    if (user1 == user2)
    {
        printf("Un usuario no puede ser amigo de si mismo\n");
        return;
    }

    // se obtiene la lista de amigos de user1
    Edge *friends_of_user1 = graph->adyacent_friendship_list[user1];
    // se guardan los amigos totales del user1
    int current_size1 = graph->friends_count[user1];
    // redimensiona el arrelgo dinamico friends_of_user1 para añadir un nuevo amigo (conexion)
    friends_of_user1 = realloc(friends_of_user1, sizeof(Edge) * (current_size1 + 1));
    // añade como destino en la posicion nueva del arreglo, al user2 para que se conecten
    friends_of_user1[current_size1].dest = user2;
    // añade el peso que tiene la conexion al amigo
    friends_of_user1[current_size1].weight = weight;
    // la lista de adyacencia del grafo en la posicion user1, contendra el arreglo de estructura edge que contiene las conexiones
    graph->adyacent_friendship_list[user1] = friends_of_user1;

    Edge *friends_of_user2 = graph->adyacent_friendship_list[user2];
    friends_of_user2 = realloc(friends_of_user2, sizeof(Edge) * (current_size1 + 1));
    friends_of_user2[current_size1].dest = user1;
    friends_of_user2[current_size1].weight = weight;
    graph->adyacent_friendship_list[user2] = friends_of_user2;
}

void remove_user(Graph *graph, int user_id)
{
    if (user_id < 0 || user_id >= graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }

    // aca para eliminar al usuario, se deben eliminar todas sus conexiones, ademas
    // de eliminar sus conexiones, se debe poder acceder al lugar donde esta el usuario con una
    // tabla hash

    graph->users_number--;

    printf("Usuario eliminado correctamente\n");
}

void display_friends(Graph *graph, int user_id)
{
    if (user_id < 0 || user_id >= graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }
}
