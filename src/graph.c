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
    // funcion auxiliar que primero elimina todas las conexiones del usuario
    remove_all_friendships(graph, user_id);
    remove_reference_to_user(graph, user_id);

    free(graph->adyacent_friendship_list[user_id]);
    graph->users_number--;

    // mueve la lista de adyacencia posteriores al usuario eliminado
    for (int i = user_id; i < graph->users_number - 1; i++)
    {
        graph->adyacent_friendship_list[i] = graph->adyacent_friendship_list[i + 1];
        graph->friends_count[i] = graph->friends_count[i + 1];
    }

    printf("Usuario eliminado correctamente\n");
}

void display_friends(Graph *graph, int user_id)
{
    if (user_id < 0 || user_id >= graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }
    printf("Los amigos del usuario con id %d son:\n", user_id);

    // esto apuntara al arreglo del tipo Edge de amigos del user
    Edge *friends_of_user = graph->adyacent_friendship_list[user_id];

    // numero de amigos actuales de este usuario
    int friends_count = graph->friends_count[user_id];

    // ahora recorre el arreglo dinamico friends_of_user para ver los amigos del usuario
    // y se imprimen

    for (int i = 0; i < friends_count; i++)
    {
        printf("Amigo ID: %d, Peso de la conexión: %d\n", friends_of_user[i].dest, friends_of_user[i].weight);
    }

    if (friends_count == 0)
    {
        printf("El usuario con ID %d no tiene amigos\n", user_id);
    }
}

void remove_friendship(Graph *graph, int user1_id, int user2_id)
{
    if (user1_id < 0 || user1_id >= graph->users_number || user2_id < 0 || user2_id >= graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }

    // se obtiene la lista de amigos del user1
    Edge *current_friendship_list = graph->adyacent_friendship_list[user1_id];
    int friends_count = graph->friends_count[user1_id];

    // se busca la amistad
    for (int i = 0; i < friends_count; i++)
    {
        if (current_friendship_list[i].dest == user2_id)
        {
            // se mueve al ultimo elemento el lugar eliminado
            current_friendship_list[i] = current_friendship_list[friends_count - 1];

            // se reduce el tamaño del arreglo dinamico con realloc
            current_friendship_list = realloc(current_friendship_list, sizeof(Edge) * (friends_count - 1));
            graph->adyacent_friendship_list[user1_id] = current_friendship_list;

            // se actualiza el contador de amigos
            graph->friends_count[user1_id]--;

            printf("La amistad entre el usuario %d y el usuario %d ha sido eliminada.\n", user1_id, user2_id);
            return;
        }
    }

    // no hay amistad
    printf("No existe una amistad entre el usuario %d y el usuario %d.\n", user1_id, user2_id);
}

void remove_all_friendships(Graph *graph, int user_id)
{
    if (user_id < 0 || user_id > graph->users_number)
    {
        printf("El usuario no existe\n");
        return;
    }

    // se obtiene la lista de amigos del usuario
    Edge *current_friendship_list = graph->adyacent_friendship_list[user_id];
    int friends_count = graph->friends_count[user_id];
    // se libera la lista dinamica de amigos
    free(current_friendship_list);
    // se coloca en NULL la referencia a la lista
    graph->adyacent_friendship_list[user_id] = NULL;
    graph->friends_count[user_id] = 0;
    printf("Todas las amistades del usuario %d han sido eliminadas.\n", user_id);
}

void remove_reference_to_user(Graph *graph, int user_id)
{
    // recorrera toda la lista buscando las referencias
    for (int i = 0; i < graph->users_number; i++)
    {
        // se salta al usuario que se esta eliminando
        if (i != user_id)
        {
            // se obtiene la lista de amigos del usuario actual (i)
            Edge *friend_list = graph->adyacent_friendship_list[i];
            int friend_count = graph->friends_count[i];

            for (int j = 0; j < friend_count; j++)
            {
                // se ve si existe la amistad
                if (friend_list[j].dest == user_id)
                {
                    // mueve el ultimo elemento al lugar del eliminado
                    friend_list[j] = friend_list[friend_count - 1];

                    // redimensionar la lista
                    friend_list = realloc(friend_list, sizeof(Edge) * (friend_count - 1));
                    graph->adyacent_friendship_list[i] = friend_list;

                    graph->friends_count[i]--;
                    break;
                }
            }
        }
    }
}
