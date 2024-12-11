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
#include "hash_table.h"

#define MAX_CHAR 256

#define COLOR_BLACK "\033[0;30m"
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


unsigned int jenkins_hash(char* key);

int get_option(int argc, char *argv[]);

int line_number_in_file(char *file);

#endif
