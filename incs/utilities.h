#ifndef UTILITIES_H
#define UTILITIES_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


unsigned int jenkins_hash(char* key);

int get_option(int argc, char *argv[]);

#endif