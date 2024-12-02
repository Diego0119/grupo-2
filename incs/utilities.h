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

unsigned int jenkins_hash(char* key);

int get_option(int argc, char *argv[]);

#endif
