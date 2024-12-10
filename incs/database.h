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


typedef struct _pending_connections {
    char *username;
    char **followers;
    int numFollowers;
    char **following;
    int numFollowing;
    struct _pending_connections *next;
} PendingConnections;

/* FUNCIONES DE CARGA Y GUARDADO DE LA BASE DE DATOS */
void create_database_dir(void);
void save_user_data(User user, GlobalInterests globalInterests);
void save_all_users(Graph graph, GlobalInterests globalInterests);
void clear_database(void);
void confirm_and_cleanup(void);
int database_exists_and_not_empty(void);
void load_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests);
static PendingConnections* load_user_from_file(const char *filename, PtrToHashTable table, Graph graph, GlobalInterests globalInterests);

/* FUNCIONES DE SESION ACTUAL */
void login(char *username, char *password, PtrToHashTable graph);
void logout(void);
User current_session(PtrToHashTable graph);

#endif
