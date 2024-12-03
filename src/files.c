#include "files.h"

int line_number_in_file(char *file) {
    int line_number = 0;
    FILE *file_pointer = fopen(file, "r");
    if (!file_pointer) {
        printf("ERROR: No se pudo abrir el archivo\n");
        exit(EXIT_FAILURE);
    }
    char c;
    while ((c = fgetc(file_pointer)) != EOF) {
        if (c == '\n') line_number++;
    }
    fclose(file_pointer);
    return line_number;
}

