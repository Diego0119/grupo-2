#include "users.h"

/**
 * @brief Crea un nuevo usuario
 * 
 * @param username Nombre de usuario
 * @param password Contraseña
 * @param name Nombre completo
 * @note Se debe iniciar sesión con el nombre de usuario y la contraseña que indique el usuario
 * @return User 
 */
User create_new_user(char* username, char* password, char* name){
    User user;
    user.id = jenkins_hash(username);
    user.username = username;
    user.password = password;
    user.name = name;
    user.posts = create_empty_userPosts();
    return user;
}

/**
 * @brief Crea lista enlazada de posts vacía
 * @note Centinela contiene fecha en que se crea la lista, o sea, cuando se creó el usuario, y el id es la cantidad de posts que tiene el usuario
 * @return UserPosts 
 */
UserPosts create_empty_userPosts(){
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
    newPost->post = content;
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
    free(posts);
}

/**
 * @brief Elimina un usuario
 * 
 * @param user usuario a eliminar
 */
void delete_user(User user){
    delete_userPosts(user.posts);
    free(user.username);
    free(user.password);
    free(user.name);
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
    printf("ID: %d\n", user.id);
    printf("Nombre: %s\n", user.name);
    printf("Usuario: %s\n", user.username);
    printf("Contraseña: %s\n", user.password);
    printf("Publicaciones:\n");
    print_userPosts(user.posts);
}
