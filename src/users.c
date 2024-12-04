/**
 * @file users.c
 * @brief Implementación de las funciones para gestionar usuarios y publicaciones
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include "users.h"
#include "graph.h"

/**
 * @brief Crea un nuevo usuario
 * 
 * @param username Nombre de usuario
 * @param password Contraseña
 * @param name Nombre completo
 * @note Se debe iniciar sesión con el nombre de usuario y la contraseña que indique el usuario
 * @return User 
 */
User create_new_user(char* username, char* password, char* name, PtrToHashTable table, Graph graph){
    if (search_in_hash_table(table, username)) {
        printf("Error: El nombre de usuario '%s' ya existe\n", username);
        return NULL;
    }

    User user = (User)malloc(sizeof(_User));
    if(!user){
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    user->id = jenkins_hash(username);
    
    user->username = strdup(username);
    user->password = strdup(password);
    user->name = strdup(name);
    user->posts = create_empty_userPosts();

    user->following = init_empty_edge();
    user->followers = init_empty_edge();
    user->numFollowing = 0;
    user->numFollowers = 0;

    user->popularity = 0;
    
    insert_into_hash_table(table, username, user);
    add_user_to_graph(graph, user);
    
    
    return user;
}

/**
 * @brief Crea lista enlazada de posts vacía
 * @note Centinela contiene fecha en que se crea la lista, o sea, cuando se creó el usuario, y el id es la cantidad de posts que tiene el usuario
 * @return UserPosts 
 */
UserPosts create_empty_userPosts(void) {
    UserPosts posts=(UserPosts)malloc(sizeof(PostNode));
    if(!posts){
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    time_t t = time(NULL);

    posts->id = 0;
    posts->date = *localtime(&t);
    posts->post = NULL;
    posts->next = NULL;
    return posts;
}

/**
 * @brief Crea un post en la lista de posts
 * 
 * @param posts Lista de posts del usuario
 * @param content Contenido del post a crear
 * @note Inserta al principio de la lista y guarda la fecha de la máquina en el momento de crear el post.
 * @return PtrToPostNode 
 */
PtrToPostNode insert_post(UserPosts posts, char* content){
    PtrToPostNode newPost = (PtrToPostNode)malloc(sizeof(PostNode));
    if(!newPost){
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    time_t t= time(NULL);
    newPost->id = jenkins_hash(content);
    newPost->date = *localtime(&t);
    newPost->post = strdup(content);
    newPost->next = posts->next;
    posts->next = newPost;
    posts->id++;
    return newPost;
}

/**
 * @brief Elimina la lista de posts de un usuario
 * 
 * @param posts Lista de posts
 */
void delete_userPosts(UserPosts posts){
    if(posts->next != NULL){
        delete_userPosts(posts->next);
    }
    free(posts->post);
    free(posts);
}

/**
 * @brief Elimina un usuario
 * 
 * @param user usuario a eliminar
 */
void delete_user(User user, PtrToHashTable table, Graph graph){
    delete_from_hash_table(table, user->username);
    remove_user_from_graph(graph, user);
    delete_userPosts(user->posts);
    free(user->username);
    free(user->password);
    free(user->name);
    free(user->following);
    free(user->followers);
    free(user);
}

/**
 * @brief Imprime una lista de posts
 * 
 * @param posts Lista de posts
 */
void print_userPosts(UserPosts posts){
    if(posts->next == NULL){
        printf("No hay publicaciones\n");
        return;
    }

    PtrToPostNode aux = posts->next;
    while(aux != NULL){
        printf("   ID: %d\n", aux->id);
        printf("   Fecha: %s\n", asctime(&aux->date));
        printf("   %s\n", aux->post);
        aux = aux->next;
    }
}

/**
 * @brief Imprime toda la información de un usuario
 * 
 * @param user Usuario
 */
void print_user(User user){
    printf("ID: %d\n", user->id);
    printf("Nombre: %s\n", user->name);
    printf("Usuario: %s\n", user->username);
    printf("Contraseña: %s\n", user->password);
    printf("Popularidad (%d) | Seguidores (%d) | Seguidos (%d)\n", user->popularity, user->numFollowers, user->numFollowing);
    printf("Publicaciones:\n");
    print_userPosts(user->posts);
}

/**
 * @brief Busca a un usuario según su nombre de usuario
 * 
 * @param username Nombre usuario
 * @param table Tabla hash de usuarios
 * @return User 
 */
User search_user(char* username, PtrToHashTable table){
    return search_in_hash_table(table, username);
}

/**
 * @brief Imprime los seguidores de un usuario
 * 
 * @param user Usuario
 */
void print_followers(User user){
    Edge aux = user->followers->next;
    printf("Seguidores de %s:\n", user->username);
    while(aux){
        printf("- %s\n", aux->dest->username);
        aux = aux->next;
    }
}

/**
 * @brief Imprime los seguidos de un usuario
 * 
 * @param user Usuario
 */
void print_following(User user){
    Edge aux = user->following->next;
    printf("Seguidos de %s:\n", user->username);
    while(aux){
        printf("- %s\n", aux->dest->username);
        aux = aux->next;
    }
}

/**
 * @brief Imprime todos los usuarios de la red social
 * 
 * @param graph Grafo de usuarios
 */
void print_all_users(Graph graph) {
    GraphList aux = graph->graphUsersList->next;
    printf("Usuarios (%d):\n", graph->usersNumber);
    while (aux) {
        printf("- %s\n", aux->username);
        aux = aux->next;
    }
}

/**
 * @brief Libera la memoria de todos los usuarios de la red social
 * 
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 */
void free_all_users(PtrToHashTable table, Graph graph) {
    GraphList aux = graph->graphUsersList->next;
    while (aux) {
        GraphList next = aux->next;
        delete_user(aux, table, graph);
        aux = next;
    }
}

/**
 * @brief Incrementa la popularidad de un usuario
 * 
 * @param user Puntero al usuario
 */
void increment_popularity(User user) {
    if (user) user->popularity++;
}


/**
 * @brief Sugerir usuarios populares basados en popularidad
 * 
 * @param table Tabla hash que contiene los usuarios
 */
void suggest_popular_users(HashTable *table) {
    printf("Usuarios populares:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Hashnode *current = table->buckets[i];
        while (current) {
            User u = (User)current->data;
            if (u->popularity > 10) { 
                printf("- %s (Popularidad: %d)\n", u->username, u->popularity);
            }
            current = current->next;
        }
    }
}

/**
 * @brief Ordena las publicaciones de un usuario por fecha
 * 
 * @param user Puntero al usuario
 */
void sort_posts(User user) {
    if (!user || !user->posts) return;

    int swapped;
    PtrToPostNode ptr1;
    PtrToPostNode lptr = NULL;

    do {
        swapped = 0;
        ptr1 = user->posts->next;

        while (ptr1 && ptr1->next != lptr) {
            if (difftime(mktime(&ptr1->date), mktime(&ptr1->next->date)) < 0) {
                // Intercambiar contenidos
                char *temp_content = ptr1->post;
                struct tm temp_date = ptr1->date;

                ptr1->post = ptr1->next->post;
                ptr1->date = ptr1->next->date;

                ptr1->next->post = temp_content;
                ptr1->next->date = temp_date;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

/**
 * @brief Guarda los usuarios en un archivo txt
 * 
 * @param table Tabla hash que contiene los usuarios a guardar
 * @param filename Archivo donde se almacenaran los datos
 * @note Escribe los datos de cada usuario en una linea
 */
void save_users_to_file(PtrToHashTable table, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error al abrir el archivo para guardar usuarios.\n");
        return;
    }

    PtrToUser user = NULL;
    for (int i = 0; i < table->size; i++) {
        user = table->table[i];
        while (user != NULL) {
            fprintf(file, "%d %s %s %s\n", user->id, user->username, user->password, user->name);

            // Guarda relaciones con seguidores
            Edge following = user->following;
            while (following != NULL) {
                fprintf(file, "FOLLOWING %d %d\n", user->id, following->id);
                following = following->next;
            }

            user = user->next;
        }
    }

    fclose(file);
    printf("Usuarios guardados en %s\n", filename);
}

/**
 * @brief Carga usuarios desde un archivo para reconstruir la tabla hash y el grafo
 * 
 * @param table Tabla hash que almacena usuarios cargados
 * @param filename Archivo desde donde se cargan los datos
 * @param graph Grafo de relaciones entre usuarios
 * @note Se crea tabla vacia si es que no hay
 */
void load_users_from_file(PtrToHashTable table, const char *filename, Graph graph) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir el archivo para cargar usuarios.\n");
        return;
    }

    char username[100], password[100], name[100];
    int id;

    while (fscanf(file, "%d %s %s %s", &id, username, password, name) == 4) {
        User new_user = create_new_user(username, password, name, table, graph);
        new_user->id = id;

        char line[256];
        while (fgets(line, sizeof(line), file)) {
            int following_id;
            if (sscanf(line, "FOLLOWING %d %d", &id, &following_id) == 2) {
                User following_user = search_user(following_id, table);
                if (following_user) {
                    add_user_to_graph(graph, new_user, following_user);
                }
            } else {
                break;
            }
        }
    }

    fclose(file);
    printf("Usuarios cargados desde %s\n", filename);
}