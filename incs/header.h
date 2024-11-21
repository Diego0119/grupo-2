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
    User usuarios[MAX_USERS];
    int *friendships[MAX_USERS]; // lista de adyacencia
} Graph;
