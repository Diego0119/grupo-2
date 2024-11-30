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
 * @brief Indice del programa (por hacer)
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int get_option(int argc, char *argv[]){
   
   /*int opt; // Variable para el manejo de opciones
	int opt_index = 0;

   // estructura para las opciones
	static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"search", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, ":hd:", long_options, &opt_index)) != -1){

		switch(opt){
			case 'h':
				printf("fly -d <directorio>      Ejecuta el programa sobre un directorio\n");
				return NULL;
                break;
			case 'd':
                return optarg;
                break;
			case ':':
                printf("Uso: 'fly -d <directorio>'\n'fly -h' para mostrar ayuda\n");
				return NULL;
				break;
			default:
				printf("Uso: 'fly -d <directorio>'\n'fly -h' para mostrar ayuda\n");
                return NULL;
				break;
		}
	}
    printf("Uso: 'fly -d <directorio>'\n'fly -h' para mostrar ayuda\n");
    return NULL;*/

}