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

/**
 * @brief Crea el directorio database si no existe.
 */
void create_database_dir(void);

/**
 * @brief Guarda toda la información de un usuario en "database/{username}_data".
 *
 * @param user Usuario a guardar
 * @param globalInterests Tabla de intereses globales
 */
void save_user_data(User user, GlobalInterests globalInterests);

/**
 * @brief Guarda todos los usuarios del grafo en la carpeta database.
 *
 * @param graph Grafo con usuarios
 * @param globalInterests Tabla global de intereses
 */
void save_all_users(Graph graph, GlobalInterests globalInterests);

/**
 * @brief Elimina todos los archivos y la carpeta database.
 */
void clear_database(void);

/**
 * @brief Pregunta al usuario si desea mantener o eliminar la base de datos.
 */
void confirm_and_cleanup(void);

/**
 * @brief Verifica si la carpeta database existe y contiene datos.
 *
 * @return 1 si existe y contiene al menos un archivo, 0 de lo contrario.
 */
int database_exists_and_not_empty(void);

/**
 * @brief Carga todos los usuarios desde los archivos en la carpeta database, restaurando el estado anterior.
 *        Se debe llamar antes de generar usuarios aleatorios.
 *
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 */
void load_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests);

#endif
