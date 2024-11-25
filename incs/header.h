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
    Edge *adyacent_friendship_list[MAX_USERS]; // lista de adyacencia
    int friends_count[MAX_USERS];              // contador de amigos de un usuario
} Graph;

typedef struct Edge
{
    int dest;   // usuario al que esta conectado
    int weight; // peso de la conexion

} Edge;

// funciones de los grafos
void initialize_graph(Graph *graph);
int add_user(Graph *graph, const char *name);
void add_friendship(Graph *graph, int user1, int user2, int weigth);
void remove_user(Graph *graph, int user_id);
void display_friends(Graph *graph, int user_id);
void remove_friendship(Graph *graph, int user1, int user2);
void remove_all_friendships(Graph *graph, int user_id);
void remove_reference_to_user(Graph *graph, int user_id);
