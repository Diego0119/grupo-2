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
void add_friendship(Graph *graph, int user1, int user2)
{
    if (user1 == user2)
    {
        printf("Un usuario no puede ser amigo de si mismo\n");
        return;
    }

    // puntero al arreglo de amigos del user1
    int *friends_of_user1 = graph->adyacent_friendship_list[user1];
    // se añade con realloc dinamicamente un nuevo espacio en el arreglo de amigos
    friends_of_user1 = realloc(friends_of_user1, sizeof(int) * (graph->users_number + 1));
    // se inserta el nuevo amigo en la lista del user1, graph->users_number es el indice en el cual se insertara el nuevo amigo
    friends_of_user1[graph->users_number] = user2;
    // actualiza la lista de adyacencia para que apunte a los amigos de user1
    graph->adyacent_friendship_list[user1] = friends_of_user1;

    int *friends_of_user2 = graph->adyacent_friendship_list[user2];
    friends_of_user2 = realloc(friends_of_user2, sizeof(int) * (graph->users_number + 1));
    friends_of_user2[graph->users_number] = user1;
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
