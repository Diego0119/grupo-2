/**
 * @file database.c
 * @brief Funciones para la gestión y guardado de la base de datos, incluyendo carga y guardado de usuarios y publicaciones,
 *        así como la lógica de seguir y dejar de seguir usuarios.
 * 
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include "database.h"

/**
 * @brief Crea el directorio 'database' si no existe.
 * 
 * @note El directorio 'database' se crea en el directorio desde donde se está ejecutando el programa.
 */
void create_database_dir(void){
    struct stat st = {0};
    if (stat("database", &st) == -1) {
        if (mkdir("database", 0777) == -1) {
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * @brief Guarda toda la información de un usuario en la base de datos
 * @note Se guardan los en la carpeta "database/{username}_data". Su info como usuario está en "database/{username}_data/data.dat", sus seguidores en "database/{username}_data/following.dat" y sus seguidores en "database/{username}_data/followers.dat". Sus publicaciones estarán en "database/{username}_data/posts", cada post siendo un archivo teniendo de nombre la fecha en formato "AAAA-MM-DD HH:MM:SS".
 *
 * @param user Usuario a guardar
 * @param globalInterests Tabla de intereses globales
 */
void save_user_data(User user, GlobalInterests globalInterests){
    
    if (!user) return;
    char filename[512];
    struct stat st = {0};
    FILE *fp;
    Edge e;
    /* crear directorio del usuario si no existe */
    snprintf(filename, sizeof(filename), "database/%s_data", user->username);
    if (stat(filename, &st) == -1) {
        if (mkdir(filename, 0777) == -1) {
            printf("ERROR: No se pudo crear el directorio '%s'\n", filename);
            exit(EXIT_FAILURE);
        }
    }
    /* guardar datos */
    snprintf(filename, sizeof(filename), "database/%s_data/data.dat", user->username);
    fp = fopen(filename, "w");
    if(!fp){
        printf("ERROR: No se pudo guardar el archivo '%s'\n", filename);
        return;
    }
    fprintf(fp, "%d\n", user->id);
    fprintf(fp, "%s\n", user->username);
    fprintf(fp, "%s\n", user->password);
    fprintf(fp, "%s\n", user->name);
    fprintf(fp, "%d\n", user->popularity);
    // Amigabilidad y categoría
    float f = calculate_friendliness(user);
    const char *cat = classify_friendliness(f);
    fprintf(fp, "%.2f\n", f); /*friendliness*/
    fprintf(fp, "%s\n", cat); /*category*/
    for (int i = 0; i < globalInterests.numInterests; i++) {
        fprintf(fp, "%d\n", user->interests[i].value); /*interests*/
    }
    fclose(fp);

    /* seguidos */
    snprintf(filename, sizeof(filename), "database/%s_data/following.dat", user->username);
    fp = fopen(filename, "w");
    if(!fp){
        return;
    }
    e = user->following->next;
    while (e) {
        fprintf(fp, "  %s\n", e->dest->username);
        e = e->next;
    }
    fclose(fp);

    /* seguidores */
    snprintf(filename, sizeof(filename), "database/%s_data/followers.dat", user->username);
    fp = fopen(filename, "w");
    if(!fp){
        return;
    }
    e = user->followers->next;
    while (e) {
        fprintf(fp, "  %s\n", e->dest->username);
        e = e->next;
    }
    fclose(fp);

    /* posts */

    /* crear directorio de posts si no existe */
    snprintf(filename, sizeof(filename), "database/%s_data/posts", user->username);
    if (stat(filename, &st) == -1) {
        if (mkdir(filename, 0777) == -1) {
            exit(EXIT_FAILURE);
        }
    }
    delete_all_in_directory(filename); /* borrar archivos dentro para evitar problemas de colisiones */

    PtrToPostNode postAux = user->posts->next;
    while (postAux) {
        char buffer[128];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &postAux->date);
        snprintf(filename, sizeof(filename), "database/%s_data/posts/%s.dat", user->username, buffer);
        int i=1;
        while(stat(filename, &st) == 0){
            snprintf(filename, sizeof(filename), "database/%s_data/posts/%s-%d.dat", user->username, buffer, i);
            i++;
        }
        fp=fopen(filename,"a");
        fprintf(fp, "%d\n", postAux->id); // postID
        fprintf(fp, "%s\n", buffer); // fecha
        if(postAux->post!=NULL){
            fprintf(fp, "%s\n", postAux->post); // contenido
        }
        postAux = postAux->next;
        fclose(fp);
        
    }

}

/**
 * @brief Guarda todos los usuarios cargados en el programa en la base de datos.
 * 
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 */
void save_all_users(Graph graph, GlobalInterests globalInterests) {
    create_database_dir();
    PtrToUser currentUser = graph->graphUsersList->next;
    while (currentUser) {
        save_user_data(currentUser, globalInterests);
        currentUser = currentUser->next;
    }

}

/**
 * @brief Elimina la base de datos en su totalidad.
 * 
 * @param graph Grafo de usuarios
 */
void clear_database(Graph graph) {
    GraphList aux = graph->graphUsersList->next;
    while (aux) {
        delete_account(aux);
        aux = aux->next;
    }
    printf("Se han eliminado todos los datos de la base de datos.\n");
}

/**
 * @brief Verifica si la carpeta database existe y no está vacía.
 */
int database_exists_and_not_empty(void) {
    DIR *dir = opendir("database");
    if (!dir) {
        return 0;
    }
    const struct dirent *entry;
    int has_files = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0) {
            has_files = 1;
            break;
        }
    }
    closedir(dir);
    return has_files;
}

/**
 * @brief Carga un usuario desde un archivo
 * 
 * @param filename Nombre del archivo
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 * @return User Usuario cargado
 */
User load_user_from_file(const char *filename, PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    char buffer[128];
    sprintf(buffer, "%s/data.dat", filename);
    FILE *fp = fopen(buffer,"r");
    if (!fp) {
        return NULL;
    }
    int id = 0;
    char username[256];
    char password[256];
    char name[256];
    int popularity;
    float friendliness;
    char category_buf[256];
    InterestTable tempInterests = init_user_interests(globalInterests);
    UserPosts posts = create_empty_userPosts();
    
    if (fscanf(fp, "%d", &id)==0) exit(EXIT_FAILURE);
    if (fscanf(fp, "%255s", username)==0) exit(EXIT_FAILURE);
    if (fscanf(fp, "%255s", password)==0) exit(EXIT_FAILURE);
    fgetc(fp);
    if (fgets(name, sizeof(name), fp) == NULL) exit(EXIT_FAILURE);
    name[strcspn(name, "\n")] = '\0';
    if (fscanf(fp, "%d", &popularity)==0) exit(EXIT_FAILURE);
    if (fscanf(fp, "%f", &friendliness)==0) exit(EXIT_FAILURE);
    fgetc(fp);
    if (fgets(category_buf, sizeof(category_buf), fp) == NULL) exit(EXIT_FAILURE);
    category_buf[strcspn(category_buf, "\n")] = '\0';

    /* carga intereses*/
    int i = 0;
    while (fscanf(fp, "%d", &tempInterests[i].value) == 1 && i<globalInterests.numInterests-1) {
        tempInterests[i].name = globalInterests.interestsTable[i];
        i++;
    }
    fclose(fp);
    
    /* carga publicaciones */
    snprintf(buffer, sizeof(buffer), "%s/posts", filename);
    DIR *dir = opendir(buffer);
    if (!dir) {
        return NULL;
    }
   
    const struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;
        char path[512];
        snprintf(path, sizeof(path), "%s/posts/%s", filename, entry->d_name);
        fp = fopen(path, "r"); 
        if (!fp) {
            continue;
        }
        int post_id;
        struct tm post_date;
        char post_content[1024];
        if (fscanf(fp, "%d\n", &post_id)==0) exit(EXIT_FAILURE);
        fgetc(fp);
        if (fgets(buffer, sizeof(buffer), fp) == NULL) exit(EXIT_FAILURE);
        strptime(buffer, "%Y-%m-%d %H:%M:%S", &post_date);
        if (fgets(post_content, sizeof(post_content), fp)==0) exit(EXIT_FAILURE);
        post_content[strcspn(post_content, "\n")] = '\0';
        fclose(fp);
        insert_post(posts, post_content);
        posts->next->id = post_id;
        posts->next->date = post_date;
    }
    closedir(dir);

    User newUser = create_new_user(username, password, name, table, graph, globalInterests);
    if (!newUser) {
        printf("ERROR: No se pudo crear el usuario %s\n", username);
        exit(EXIT_FAILURE);
    }

    newUser->id = id;
    newUser->popularity = popularity;
    newUser->interests = tempInterests;
    newUser->posts = posts;
    newUser->friendliness = friendliness;
    newUser->category = strdup(category_buf);
    return newUser;
}

/**
 * @brief Carga todos los usuarios desde la base de datos.
 * 
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 */
void load_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests){
    DIR *dir = opendir("database");
    if (!dir) {
        perror("ERROR: No se pudo abrir el directorio database");
        return; 
    }

    const struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;

        char path[1024];
        
        snprintf(path, sizeof(path), "database/%s", entry->d_name);
        User user = load_user_from_file(path, table, graph, globalInterests);
        if(!user){
            printf("ERROR: No se pudo cargar el usuario %s\n", entry->d_name);
            exit(EXIT_FAILURE);
        }
        
    }
    
    closedir(dir);
    
}

/**
 * @brief Carga todas las conexiones (aristas) de los usuarios desde la base de datos.
 * @note Es necesario que se haya cargado los usuarios con @see load_all_users antes de llamar a esta función.
 * 
 * @param table Tabla hash de usuarios
 * @param globalInterests Tabla de intereses globales
 */
void load_connections(PtrToHashTable table, GlobalInterests globalInterests){
    DIR *dir = opendir("database");
    if (!dir) {
        return; 
    }
    const struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;
        char path[512];
        /* rescatar usuario */
        snprintf(path, sizeof(path), "database/%s/data.dat", entry->d_name);
        FILE *fp = fopen(path, "r");
        if (!fp) {
            continue;
        }
        char username[256];
        for(int i=0; i<2; i++){
            if(fscanf(fp, "%255s", username)==0) exit(EXIT_FAILURE);
        }
       

        User currentUser=search_user(username, table);
        
        if(!currentUser){
            printf("ERROR: No se pudo cargar el usuario %s\n", username);
            exit(EXIT_FAILURE);
        }
        fclose(fp);

        /* following */
        snprintf(path, sizeof(path), "database/%s/following.dat", entry->d_name);
        fp = fopen(path, "r");
        if (!fp) {
            continue;
        }
        char user2[256];
        while(fscanf(fp, "%255s", user2)!=EOF){
            add_edge(currentUser, search_user(user2, table), globalInterests);
        }
        fclose(fp);
    }
}

/**
 * @brief Carga la base de datos en su totalidad
 * 
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 */
void load_database(PtrToHashTable table, Graph graph, GlobalInterests globalInterests){
    load_all_users(table, graph, globalInterests);
    load_connections(table, globalInterests);
}

/**
 * @brief Obtiene el usuario de la sesión iniciada actualmente, si existe.
 * 
 * @param graph Grafo de usuarios
 * @return User Usuario actual. NULL si la sesión no existe.
 */
User current_session(PtrToHashTable graph) {
    FILE *file = fopen("current.dat","r");
    if (!file) {
        return NULL;
    }
    
    char userTemp[256];
    if (fscanf(file,"%255s",userTemp)!=1) {
        fclose(file);
        return NULL;
    }
    fclose(file);

    User user = search_user(userTemp, graph);
    if (!user) {
        logout();
        return NULL;
    }

    return user;
}

/**
 * @brief Inicia sesión
 * 
 * @param graph Grafo de usuarios
 * @note Se guarda el usuario actual en "current.dat"
 */
void login(PtrToHashTable graph) {
    FILE *file = fopen("current.dat","w");
    if (!file) {
        printf("ERROR: No se pudo iniciar sesión.\n");
        return;
    }

    print_logo();

    // leer usuario y contraseña
    char *username=malloc(sizeof(char)*256);
    char *password=malloc(sizeof(char)*256);
    printf("Ingrese su nombre de usuario: ");
    if (scanf("%255s",username)==0) exit(EXIT_FAILURE);
    printf("Ingrese su contraseña: ");
    if (scanf("%255s",password)==0) exit(EXIT_FAILURE);

    User user = search_user(username, graph);
    if (!user) {
        printf(COLOR_RED COLOR_BOLD"ERROR: Usuario no existe. Intente nuevamente\n"COLOR_RESET);
        free(username);
        free(password);
        fclose(file);
        return;
    }

    if (strcmp(user->password, password) != 0) {
        printf("ERROR: Contraseña incorrecta.\n");
        fclose(file);
        free(username);
        free(password);
        return;
    }

    fprintf(file,"%s",username);
    printf("Sesión iniciada correctamente como '%s'.\n", username);
    free(username);
    free(password);
    fclose(file);
}

/**
 * @brief Cierra sesión
 * @note Borra el archivo "current.dat"
 */
void logout(void) {
    if(remove("current.dat")==0){
        print_logo();
        printf(COLOR_RED COLOR_BOLD"Se ha cerrado sesión exitosamente. Nos vemos pronto...\n"COLOR_RESET);
    }
}

/**
 * @brief Registra un nuevo usuario en la base de datos
 * 
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 * 
 */
void register_user(PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {

    print_logo();

    char *username=malloc(sizeof(char)*256);
    printf("Ingrese su nombre de usuario: ");
    if (scanf("%255s",username)==0) exit(EXIT_FAILURE);
    User user = search_user(username, table);
    while(user) {
        printf(COLOR_RED COLOR_BOLD"ERROR: El nombre de usuario '%s' ya existe. Intente con otro usuario: \n"COLOR_RESET, username);
        if (scanf("%255s",username)==0) exit(EXIT_FAILURE);
        user = search_user(username, table);
    }

    char *password=malloc(sizeof(char)*256);
    printf("Ingrese su contraseña: ");
    if (scanf("%255s",password)==0) exit(EXIT_FAILURE);

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer
    char *name=malloc(sizeof(char)*256);
    printf("Ingrese su nombre: ");
    if (fgets(name, 256, stdin) != NULL) {
        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        }
    }

    user = create_new_user(username, password, name, table, graph, globalInterests);
    if(!user){
        printf(COLOR_RED COLOR_BOLD"ERROR: No se pudo crear el usuario '%s'.\n"COLOR_RESET, username);
    }
    free(username);
    free(password);
    free(name);

    printf("Bienvenido/a %s a DevGraph! Ingrese ID de intereses (0 para terminar)\n", user->name);
    print_global_interests(globalInterests);
    int option;
    do {
        if (scanf("%d",&option)==0) exit(EXIT_FAILURE);
        if(option<0 || option>=globalInterests.numInterests-1){
            printf(COLOR_RED COLOR_BOLD"ERROR: ID de interes no válido. Intente nuevamente\n"COLOR_RESET);
        }
        else if(option>0){
            add_interest(user, globalInterests, option);
            printf("Se ha añadido el interes %s.\n", globalInterests.interestsTable[option]);
        }
    } while(option != 0);

    save_user_data(user, globalInterests);
    printf("Usuario registrado correctamente. Ahora puedes iniciar sesión y conectar con otros usuarios.\n");
}

/**
 * @brief Publicar un post desde un usuario
 * 
 * @param user Usuario publicador
 * @param globalInterests Tabla de intereses globales
 * 
 * @note Se guarda la publicación en la carpeta "database/{username}_data/posts"
 */
void write_post(User user, GlobalInterests globalInterests) {
    print_logo();
    printf(COLOR_RED COLOR_BOLD"Publicando como '%s'. Escriba el contenido de la publicación:\n\n"COLOR_RESET, user->username);
    printf("-----------------------------------------------------------------------------\n");
    char *content=malloc(sizeof(char)*1028);
    if (fgets(content, 512, stdin) != NULL) {
        size_t len = strlen(content);
        if (len > 0 && content[len-1] == '\n') {
            content[len-1] = '\0';
        }
    }
    printf("-----------------------------------------------------------------------------\n");
    insert_post(user->posts, content);
    save_user_data(user, globalInterests);
    free(content);
    printf("Publicación creada correctamente.\n");
}

/**
 * @brief Seguir a un usuario
 * 
 * @param user Usuario que sigue a otro
 * @param follow Nombre del usuario que se va a seguir
 * @param globalInterests Tabla de intereses globales
 * @param table Tabla hash de usuarios
 * 
 * @note Esta información se guarda en la carpeta "database/{user}_data/following.dat"
 */
void follow(User user, char* follow, GlobalInterests globalInterests, PtrToHashTable table) {
    User to_follow = search_user(follow, table);
    if(!to_follow){
        printf("ERROR: Usuario '%s' no encontrado.\n", follow);
        return;
    }
    add_edge(user, to_follow, globalInterests);
    save_user_data(user, globalInterests);
    printf("Ahora sigues a '%s'.\n", to_follow->username);
}

/**
 * @brief Dejar de seguir a un usuario
 * 
 * @param user Usuario que deja de seguir a otro
 * @param follow Nombre del usuario que ya no se va a seguir
 * @param globalInterests Tabla de intereses globales
 * @param table Tabla hash de usuarios
 * 
 * @note Esta información se guarda en la carpeta "database/{user}_data/followers.dat"
 */
void unfollow(User user, char* follow, GlobalInterests globalInterests, PtrToHashTable table) {
    User to_unfollow = search_user(follow, table);
    if(!to_unfollow){
        printf("ERROR: '%s' no es un usuario que exista.\n", follow);
        return;
    }
    // Llamada a remove_edge
    remove_edge(user, to_unfollow);
    // Guardar datos
    save_user_data(user, globalInterests);
    printf("Has dejado de seguir a '%s'.\n", to_unfollow->username);
}

/**
 * @brief Borrar la cuenta de un usuario de la base de datos
 * 
 * @param user Usuario a borrar
 * 
 * @note Se borran todos los archivos de la carpeta "database/{user}_data" y la misma carpeta
 */
void delete_account(User user) {
    char path[512];
    snprintf(path, sizeof(path), "database/%s_data/data.dat", user->username);
    remove(path);
    snprintf(path, sizeof(path), "database/%s_data/following.dat", user->username);
    remove(path);
    snprintf(path, sizeof(path), "database/%s_data/followers.dat", user->username);
    remove(path);
    snprintf(path, sizeof(path), "database/%s_data/posts", user->username);
    delete_all_in_directory(path);
    rmdir(path);
    snprintf(path, sizeof(path), "database/%s_data", user->username);
    rmdir(path);
    printf("Se ha borrado la cuenta de '%s' exitosamente.\n", user->username);
    logout();
}

/**
 * @brief Permite a un usuario editar la información de su cuenta
 * 
 * @param user Sesión iniciada
 * @param globalInterests Tabla de intereses globales
 * @param table Tabla de usuarios
 */
void edit_account(User user, GlobalInterests globalInterests, PtrToHashTable table) {
    
    printf("Qué información de su cuenta desea modificar?\n");
    printf("1. Nombre\n2. Usuario\n3. Contraseña\n");
    int option;

    do {
        if (scanf("%d",&option)==0) exit(EXIT_FAILURE);
        if(option<0 || option>3){
            printf("ERROR: Opción inválida. Intente nuevamente\n");
        }
    } while(option>3 || option<0);
    
    switch(option){
        case 1: 
            printf("Ingrese el nuevo nombre: ");
            char *new_name=malloc(sizeof(char)*256);
            while (getchar() != '\n');
            if (fgets(new_name, 256, stdin) != NULL) {
                size_t len = strlen(new_name);
                if (len > 0 && new_name[len - 1] == '\n') {
                    new_name[len - 1] = '\0';
                }
            }
            free(user->name);
            user->name = strdup(new_name);
            printf("Se ha modificado su nombre a '%s'.\n", user->name);
            free(new_name);
            break;
        case 2:
            printf("Ingrese el nuevo usuario: ");
            char *new_username=malloc(sizeof(char)*256);
            if (scanf("%255s",new_username)==0) exit(EXIT_FAILURE);
            User user_aux = search_user(new_username, table);
            if(user_aux){
                printf("ERROR: El nombre de usuario '%s' ya existe. Intente nuevamente \n", new_username);
                free(new_username);
                return;
            }
            free(user->username);
            user->username = strdup(new_username);
            printf("Se ha modificado su nombre de usuario a '%s'.\n", user->username);
            free(new_username);
            break;

        case 3:
            printf("Ingrese la nueva contraseña: ");
            char *new_password=malloc(sizeof(char)*256);
            if (scanf("%255s",new_password)==0) exit(EXIT_FAILURE);
            free(user->password);
            user->password = strdup(new_password);
            printf("Se ha modificado su contraseña a '%s'.\n", user->password);
            free(new_password);
            break;
    }
    save_user_data(user, globalInterests);
}

/**
 * @brief Genera en su totalidad una base de datos
 * 
 * @param quantity Cantidad de usuarios a generar
 * @param table Tabla hash de usuarios
 * @param graph Grafo de usuarios
 * @param globalInterests Tabla de intereses globales
 */
void generate_database(int quantity, PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    generate_users(quantity, table, graph, globalInterests);
    generate_random_connections(graph, globalInterests);
    generate_posts_for_everyone(graph, globalInterests);
    save_all_users(graph, globalInterests);
}