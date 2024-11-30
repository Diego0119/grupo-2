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
typedef PostNode* PtrToPostNode;
typedef PtrToPostNode PostsPosition;
typedef PtrToPostNode UserPosts;

/*grafo*/
typedef struct _edge* Edge;
typedef struct _graph* Graph;

#define MAX_POST_TABLE 100

/**
 * @struct _postNode
 * @brief Estructura que define un post (publicación)
 * @note Es el nodo de una lista enlazada simple
 */
struct _postNode {
    int id; /*!< id (hash) del post */
    struct tm date; /*!< fecha del post */
    char* post; /*!< contenido del post */
    PtrToPostNode next; /*!< Puntero al post siguiente */
};

/**
 * @struct _user
 * @brief Estructura que almacena los datos de un usuario
 */
struct _user{
    int id; /*!< id (hash) del usuario*/
    char *username; /*!< username*/
    char *password; /*!< contraseña del usuario*/
    char *name; /*!< nombre del usuario*/
    UserPosts posts; /*!< puntero a la lista de posts*/
    /* GRAFO */
    Edge following; // usuarios que sigue
    Edge followers; // usuarios que lo siguen
    int numFollowing; // numero de usuarios que sigue
    int numFollowers; // numero de usuarios que lo siguen
    PtrToUser next; /* siguiente en la lista del grafo */
    /* INTERESES */
    //bool interests[]; /*POR HACER*/
};

/**
 *  @struct _postNode
 *  @brief Estructura que define un post (publicación)
 *  @note Es el nodo de una lista enlazada simple
 */
struct _postNode {
    int id; /*!< id (hash) del post*/
    struct tm date; /*!< fecha del post*/
    char* post; /*!< contenido del post*/
    PtrToPostNode next; /*!< Puntero al post siguiente*/
};

// Funciones para gestionar usuarios
User create_new_user(char* username, char* password, char* name, PtrToHashTable table, Graph graph);
void delete_user(User user, PtrToHashTable table, Graph graph);
User search_user(char* username, PtrToHashTable table);
void free_all_users(PtrToHashTable table, Graph graph); 

// funciones de impresión
void print_user(User user);
void print_all_users(Graph graph);
void print_followers(User user);
void print_following(User user);

UserPosts create_empty_userPosts();
PtrToPostNode insert_post(UserPosts posts, char* content);
PtrToPostNode search_post(UserPosts posts, int postId);
void delete_post(UserPosts posts, int postId); /*PENDIENTE*/
void delete_userPosts(UserPosts posts);
void print_userPosts(UserPosts posts);

#endif
