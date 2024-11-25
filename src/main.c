#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"

int main(int argc, char *argv[]){

    User user = create_new_user(argv[1], argv[2], argv[3]);

    insert_post(user.posts, "Hola mundooo");

    insert_post(user.posts, "chill de cojones");

    print_user(user);

    delete_user(&user);

    return 0;

    
}