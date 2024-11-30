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
#include "utilities.h" // Para jenkins_hash

/**
 * @brief Crea un nuevo usuario
 * 
 * @param username Nombre de usuario
 * @param password Contraseña
 * @param name Nombre completo
 * @note Se debe iniciar sesión con el nombre de usuario y la contraseña que indique el usuario
 * @return User Usuario creado
 */
User create_new_user(char* username, char* password, char* name, PtrToHashTable table, Graph graph){
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
    
    insert_into_hash_table(table, username, user);
    add_user_to_graph(graph, user);
    
    
    return user;
}



/**
 * @brief Crea una lista enlazada de posts vacía
 * @note Centinela contiene fecha en que se crea la lista
 * @return UserPosts Lista enlazada vacía
 */
UserPosts create_empty_userPosts() {
    UserPosts posts = (UserPosts)malloc(sizeof(PostNode));
    if (!posts) {
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    time_t t = time(NULL);
    posts->id = 0; // ID inicial de los posts
    posts->date = *localtime(&t); // Fecha de creación
    posts->post = NULL;
    posts->next = NULL;
    return posts;
}

/**
 * @brief Inserta un nuevo post en la lista de publicaciones de un usuario
 * 
 * @param posts Lista de posts
 * @param content Contenido del post
 * @return PtrToPostNode Nodo del post creado
 */
PtrToPostNode insert_post(UserPosts posts, char* content) {
    PtrToPostNode newPost = (PtrToPostNode)malloc(sizeof(PostNode));
    if (!newPost) {
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    time_t t = time(NULL);
    newPost->id = jenkins_hash(content); // Generar ID único basado en el contenido
    newPost->date = *localtime(&t);
    newPost->post = strdup(content);
    newPost->next = posts->next; // Inserción al inicio de la lista
    posts->next = newPost;
    posts->id++; // Incrementa el contador de publicaciones
    return newPost;
}

/**
 * @brief Busca un post específico por ID en la lista de publicaciones
 * 
 * @param posts Lista de publicaciones
 * @param postId ID del post a buscar
 * @return PtrToPostNode Nodo del post encontrado o NULL si no existe
 */
PtrToPostNode search_post(UserPosts posts, int postId) {
    PtrToPostNode current = posts->next; // Saltar el centinela
    while (current != NULL) {
        if (current->id == postId) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief Elimina un post específico de la lista enlazada
 * 
 * @param posts Lista de publicaciones
 * @param postId ID del post a eliminar
 */
void delete_post(UserPosts posts, int postId) {
    PtrToPostNode current = posts;
    PtrToPostNode previous = NULL;

    while (current != NULL) {
        if (current->id == postId) {
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                posts->next = current->next; // Caso en el que sea el primer nodo
            }
            free(current->post);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Post con ID %d no encontrado.\n", postId);
}

/**
 * @brief Elimina toda la lista de posts
 * 
 * @param posts Lista de posts a eliminar
 */
void delete_userPosts(UserPosts posts) {
    if (posts->next != NULL) {
        delete_userPosts(posts->next);
    }
    free(posts->post);
    free(posts);
}

/**
 * @brief Elimina un usuario completo, incluyendo su lista de posts
 * 
 * @param user Usuario a eliminar
 */
void delete_user(User *user){
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
void print_userPosts(UserPosts posts) {
    if (posts->next == NULL) {
        printf("No hay publicaciones\n");
        return;
    }

    PtrToPostNode aux = posts->next;
    while (aux != NULL) {
        printf("   ID: %d\n", aux->id);
        printf("   Fecha: %s", asctime(&aux->date));
        printf("   Contenido: %s\n", aux->post);
        aux = aux->next;
    }
}

/**
 * @brief Imprime toda la información de un usuario
 * 
 * @param user Usuario
 */
void print_user(User user){
    printf("ID: %d\n", user.id);
    printf("Nombre: %s\n", user.name);
    printf("Usuario: %s\n", user.username);
    printf("Contraseña: %s\n", user.password);
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