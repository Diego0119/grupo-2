#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"
#include "heaps.h"

int main(int argc, char *argv[]){

    User user = create_new_user(argv[1], argv[2], argv[3]);

    insert_post(user.posts, "Hola mundooo");

    insert_post(user.posts, "chill de cojones");

    print_user(user);

    delete_user(&user);

    // EJEMPLO DE PUBLICACIONES CON COLAS DE PRIORIDAD (HEAPS)

    heap h = {0}; // Creamos un heap vacío (Cola de prioridad)

    // Insertar publicaciones
    insert_new_post(&h, "pepito123", 50, "hoy comi pancito, amo la cocina!"); // cola de prioridad, usuario, prioridad, contenido
    insert_new_post(&h, "masterjoy", 75, "soy tan suertudo, amo apostar");
    insert_new_post(&h, "jotabond12", 95, "que mala suerte, como odio los dias lunes");
    insert_new_post(&h, "userDesk", 65, "a veces me gusta correr");
    insert_new_post(&h, "Charmy404", 85, "amo jugar a la consola");

    print_heap(&h);

    ver_posts(&h);

    print_heap(&h);

    return 0;
}