#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_USERS 100 // maxima cantidad de users en la red

// estructura de usuarios provisoria (ivan la hara )
typedef struct User
{
    char name[50];
} User;

typedef struct Graph
{
    int users_number;
    User users[MAX_USERS];
    int *adyacent_friendship_list[MAX_USERS]; // lista de adyacencia
} Graph;

// funciones de los grafos
void initialize_graph(Graph *graph);
int add_user(Graph *graph, const char *name);
void add_friendship(Graph *graph, int user1, int user2);
