/**
 * @file database.h
 * @brief Cabecera para database.c
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#include "users.h"
#include "graph.h"
#include "utilities.h"

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
 * @struct _pending_connections
 * @brief Lista auxiliar para guardar las conexiones pendientes de un usuario en la carga de la base de datos
 */
typedef struct _pending_connections {
    char *username; /*!< Nombre de usuario */
    char **followers; /*!< Nombres de los usuarios que lo siguen */
    int numFollowers; /*!< Numero de seguidores */
    char **following; /*!< Nombres de los usuarios que sigue */
    int numFollowing; /*!< Número de seguidos */
    struct _pending_connections *next; /*!< Siguiente en la lista */
} PendingConnections;

/* FUNCIONES DE CARGA Y GUARDADO DE LA BASE DE DATOS */
void create_database_dir(void);
void save_user_data(User user, GlobalInterests globalInterests);
void save_all_users(Graph graph, GlobalInterests globalInterests);
void clear_database(void);
void confirm_and_cleanup(void);
int database_exists_and_not_empty(void);
void load_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
User load_user_from_file(const char *filename, PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
void load_database(PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
void load_connections(PtrToHashTable table, GlobalInterests globalInterests);

/* FUNCIONES DE SESION ACTUAL */
void login(PtrToHashTable graph);
void logout(void);
User current_session(PtrToHashTable graph);
void register_user(PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
void write_post(User user, GlobalInterests globalInterests);
void follow(User user, char* follow, GlobalInterests globalInterests, PtrToHashTable table);
void unfollow(User user, char* follow, GlobalInterests globalInterests, PtrToHashTable table);
void delete_account(User user);
void edit_account(User user, GlobalInterests globalInterests, PtrToHashTable table);

#endif
