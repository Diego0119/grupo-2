/**
 * @file users.h
 * @brief Cabecera para users.c
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#ifndef USERS_H
#define USERS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utilities.h"
#include "hash_table.h"
#include "graph.h"

/*usuarios individuales*/
typedef struct _user _User;
typedef _User *User;
typedef _User *PtrToUser;

typedef struct _postNode PostNode;
typedef PostNode *PtrToPostNode;
typedef PtrToPostNode PostsPosition;
typedef PtrToPostNode UserPosts;

typedef struct _interest Interest;
typedef Interest *InterestTable;
typedef Interest *PtrToInterest;

/* typedefs para evitar errores de compilación */
typedef struct _globalInterests GlobalInterests;
typedef struct _edge *Edge;
typedef struct _graph *Graph;


/**
 * @struct _user
 * @brief Estructura que almacena los datos de un usuario
 */
struct _user{
    int id;          /*!< id (hash) del usuario*/
    char *username;  /*!< username*/
    char *password;  /*!< contraseña del usuario*/
    char *name;      /*!< nombre del usuario*/
    UserPosts posts; /*!< puntero a la lista de posts*/ 
    /* GRAFO */
    Edge following;   /*!< lista de adyacencia de usuarios que sigue*/
    Edge followers;   /*!< lista de adyacencia de usuarios que sigue*/
    int numFollowing; /*!< número de usuarios que sigue */
    int numFollowers; /*!< número de usuarios que lo siguen */
    PtrToUser next;   /* siguiente en la lista del grafo */
    /* INTERESES Y POPULARIDAD*/
    int popularity;          /*!< popularidad del usuario */
    InterestTable interests; /* Tabla de intereses del usuario */

    // Campos nuevos para amigabilidad
    float friendliness;  
    char *category;      
};

/**
 *  @struct _postNode
 *  @brief Estructura que define un post (publicación)
 *  @note Es el nodo de una lista enlazada simple
 */
struct _postNode{
    int id;             /*!< id (hash) del post*/
    struct tm date;     /*!< fecha del post*/
    char *post;         /*!< contenido del post*/
    int priority;       /*!< prioridad del post*/
    PtrToPostNode next; /*!< Puntero al post siguiente*/
};

/**
 * @struct _interest
 * @brief Estructura que almacena un interes
 */
struct _interest{
    char **name; /*!< Nombre del interes*/
    int globalId; /*!< Index en la tabla de intereses globales */
    int value; /*!< Valor del interes (0 si no la tiene, 1 si la tiene) */
};

/**
 * @struct _globalInterests
 * @brief Estructura que almacena la tablas de intereses globales
 */
struct _globalInterests{
    char **interestsTable; /*!< Tabla de intereses globales */
    int numInterests; /*!< Número de intereses totales */
};

// Funciones para gestionar usuarios
User create_new_user(char *username, char *password, char *name, PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
void delete_user(User user, PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
User search_user(char *username, PtrToHashTable table);
void free_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
void increment_popularity(User user);

// funciones de impresión
void print_user(User user, GlobalInterests globalInterestsTable);
void print_all_users(Graph graph);
void print_followers(User user);
void print_following(User user);
void print_user_interests(InterestTable userInterests, GlobalInterests globalInterestsTable);
void suggest_popular_users(HashTable *table);

// Funciones para gestionar publicaciones
UserPosts create_empty_userPosts(void);
PtrToPostNode insert_post(UserPosts posts, char *content);
PtrToPostNode search_post(UserPosts posts, int postId);
void delete_post(UserPosts posts, int postId); /*PENDIENTE*/
void delete_userPosts(UserPosts posts);
void print_userPosts(UserPosts posts);
void sort_posts(User user);

// funciones de intereses
GlobalInterests init_global_interests(void);
void free_global_interests(GlobalInterests globalInterestTable);
InterestTable init_user_interests(GlobalInterests globalInterestTable);
void free_user_interests(InterestTable userInterests);
void add_interest(User user, GlobalInterests globalInterestTable, int interestId);
void print_global_interests(GlobalInterests globalInterestTable);
double edge_jaccard(User user1, User user2, GlobalInterests globalInterestTable);

// generar conexiones y usuarios
void generate_random_connections(Graph graph, GlobalInterests globalInterests);
void generate_users(int quantity, PtrToHashTable table, Graph graph, GlobalInterests globalInterests);

float calculate_friendliness(User user);
void print_friendliness_table(Graph graph);
const char *classify_friendliness(float friendliness);
#endif
