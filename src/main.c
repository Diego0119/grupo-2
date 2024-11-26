#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "users.h"
#include "hash_table.h"

int main(){

    // prueba usuarios + tabla hash

    PtrToHashTable userTable = create_hash_table(HASH_TABLE_SIZE);
    User user1 = create_new_user("ivan","123","aaaa", userTable);
    User user2 = create_new_user("adasda","1234","asidaisd", userTable);

    insert_post(user1->posts, "Hola mundooo");
    insert_post(user1->posts, "chill de cojones");

    insert_post(user2->posts, "Hola mundooo");
    insert_post(user2->posts, "chill de cojones");

    User current_user=search_user("ivan", userTable);
    print_user(current_user);


    delete_user(user1, userTable);
    delete_user(user2, userTable);

    free_hash_table(userTable);

    return 0;

    
}