/**
 * @file utilities.h
 * @brief Cabecera para utilities.c
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "hash_table.h"
#include "graph.h"
#include "users.h"

#define MAX_CHAR 256

/* typedefs para evitar errores de compilación */
typedef struct _globalInterests GlobalInterests;
typedef struct _edge *Edge;
typedef struct _graph *Graph;

/* Defines para impresión */
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_WHITE "\033[0;37m"
#define COLOR_BOLD "\033[1m"
#define COLOR_UNDERLINE "\033[4m"
#define COLOR_RESET "\033[0m"
#define CLEAN_SCREEN "\033[2J"


unsigned int jenkins_hash(char* key);
int get_option(int argc, char *argv[]);
int line_number_in_file(char *file);
void delete_all_in_directory(const char *directory);
void print_logo(void);
void free_structures_and_exit(PtrToHashTable table, Graph graph, GlobalInterests globalInterestsTable);

#endif
