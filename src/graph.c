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

void add_friendship(Graph *graph, int user1, int user2)
{
    if (user1 == user2)
    {
        printf("Un usuario no puede ser amigo de si mismo\n");
        return;
    }
}