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

   return hash;
}

/**
 * @brief Indice del programa (por hacer)
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int get_option(int argc, char *argv[]){
   


}