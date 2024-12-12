/**
 * @file utilities.c
 * @brief Funciones de utilidad y testing para el programa
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */


#include "utilities.h"

/**
 * @brief Calcula el hash de un string
 * 
 * @param key 
 * @return unsigned int
 */
unsigned int jenkins_hash(char* key)
{
   unsigned int hash = 0;

   while (*key){
      hash += (unsigned char)(*key);
      hash += (hash << 10);
      hash ^= (hash >> 6);

      key++;
   }

   hash += (hash << 3);
   hash ^= (hash >> 11);
   hash += (hash << 15);

   return hash % HASH_TABLE_SIZE;
}

/**
 * @brief Indice del programa
 * 
 * @param argc 
 * @param argv 
 * @return int Retorna número de la opción seleccionada
 */
int get_option(int argc, char *argv[]) {
   
   int opt; // Variable para el manejo de opciones
	int opt_index = 0;

   // estructura para las opciones
	static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"login", no_argument, 0, 'l'},
        {"logout", no_argument, 0, 'o'},
        {"generate", required_argument, 0, 'g'},
        {"register", no_argument, 0, 'r'},
        {"post", no_argument, 0, 'p'},
        {"me", no_argument, 0, 'm'},
        {"user", required_argument, 0, 'u'},
        {"all", no_argument, 0, 'a'},
        {"follow", required_argument, 0, 'f'},
        {"unfollow", required_argument, 0, 'n'},
        {"delete", no_argument, 0, 'd'},
        {"clear", no_argument, 0, 'c'},
        {"edit", no_argument, 0, 'e'},
        {"feed", no_argument, 0, 'q'},
        {"connect", no_argument, 0, 'x'},
        {0, 0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, ":hlog:rpmu:af:n:dceqx", long_options, &opt_index)) != -1){

		switch(opt){
         // ayuda
			case 'h':
            printf("");
				printf(COLOR_RED COLOR_BOLD"COMANDOS DE DEVGRAPH\n"COLOR_RESET"  -h, --help\t\t\t Muestra esta ayuda\n  -g, --generate <cantidad>\t Genera usuarios aleatorios\n  -c, --clear\t\t\t Borra la base de datos\n  -a, --all\t\t\t Lista todos los usuarios registrados\n\n  -l, --login\t\t\t Inicia sesión\n  -o, --logout\t\t\t Cerrar sesión\n  -r, --register\t\t Registra un nuevo usuario\n  -d, --delete \t\t\t Elimina la cuenta de la sesión actual\n  -e, --edit \t\t\t Permite editar la información de la sesión actual\n\n  -p, --post\t\t\t Publica una publicación\n  -m, --me\t\t\t Muestra el perfil del usuario actual\n  -u, --user <usuario>\t\t Muestra el perfil de un usuario\n  -f, --follow <usuario>\t Sigue a un usuario\n  -n, --unfollow <usuario>\t Deja de seguir a un usuario\n\n  -q, --feed\t\t\t Muestra los posts seleccionados para ti\n  -x, --connect\t\t\t Muestra los usuarios recomendados para ti\n");
				return 0;
            break;
         // iniciar sesión
			case 'l':
            return 1;
            break;
         // cerrar sesión
			case 'o':
            return 2;
				break;
         // generar usuarios
         case 'g':
            return 3;
            break;
         // registrar usuario
         case 'r':
            return 4;
            break;
         // publicar publicación
         case 'p':
            return 5;
            break;
         // ver perfil propio
         case 'm':
            return 6;
            break;
         // ver perfil de un usuario
         case 'u':
            return 7;
            break;
         // ver todos los usuarios
         case 'a':
            return 8;
            break;
         // seguir a un usuario
         case 'f':
            return 9;
            break;
         // dejar de seguir a un usuario
         case 'n':
            return 10;
            break;
         // borrar cuenta
         case 'd':
            return 11;
            break;
         // borrar base de datos
         case 'c':
            return 12;
            break;
         // editar información del usuario
         case 'e':
            return 13;
            break;
         // no se reconoce el comando
         case 'q':
            return 14;
            break;
         case 'x':
            return 15;
            break;
			default:
				printf("'./devgraph -h' para mostrar ayuda\n");
            return -1;
				break;
		}
	}
    printf("'./devgraph -h' para mostrar ayuda\n");
    return -1;
}


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
