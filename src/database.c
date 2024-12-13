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
 * @brief Crea el directorio database si no existe.
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
 * @brief Guarda toda la información de un usuario en "database/{username}_data".
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
            exit(EXIT_FAILURE);
        }
    }
    /* guardar datos */
    snprintf(filename, sizeof(filename), "database/%s_data/data.dat", user->username);
    fp = fopen(filename, "w");
    if(!fp){
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
        fprintf(fp, "%s\n", postAux->post); // contenido
        postAux = postAux->next;
        
    }

}

/**
 * @brief Guarda todos los usuarios en la carpeta database.
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
 * @brief Elimina todos los archivos y la carpeta database.
 */
void clear_database(void) {
    DIR *dir = opendir("database");
    if (!dir) {
        return;
    }

    struct dirent *entry;
    char path[512];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;
        snprintf(path, sizeof(path), "database/%s/posts", entry->d_name);
        delete_all_in_directory(path);
        rmdir(path);
        snprintf(path, sizeof(path), "database/%s", entry->d_name);
        delete_all_in_directory(path);
        rmdir(path);
    }
    closedir(dir);

    rmdir("database");
    printf("Se han eliminado todos los datos de la base de datos.\n");
}

/**
 * @brief Pregunta si se desea mantener o eliminar la base de datos.
 */
void confirm_and_cleanup(void) {
    int opcion;
    printf("¿Desea mantener los datos guardados?\n");
    printf("1. Sí\n");
    printf("2. No, eliminar todo.\n");
    printf("Elige una opción: ");
    if (scanf("%d", &opcion) != 1) {
        printf("Entrada no válida, se conservarán los datos.\n");
        return;
    }

    if (opcion == 2) {
        clear_database();
    } else {
        printf("Se conservarán los datos.\n");
    }
}


/**
 * @brief Verifica si la carpeta database existe y no está vacía.
 */
int database_exists_and_not_empty(void) {
    DIR *dir = opendir("database");
    if (!dir) {
        return 0;
    }
    struct dirent *entry;
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
 * @brief 
 * 
 * @param str 
 */
static void trim(char *str) {
    char *start = str;
    while(*start==' ' || *start=='\t' || *start=='\n') start++;
    if(*start == 0) {
        *str = '\0';
        return;
    }

    char *end = start + strlen(start) - 1;
    while(end > start && (*end==' ' || *end=='\t' || *end=='\n'))
        end--;
    *(end+1) = '\0';

    if (start != str) memmove(str, start, end - start + 2);
}

/**
 * @brief Carga un usuario desde un archivo y devuelve su PendingConnections.
 * 
 * Se añade debug extra para entender el flujo de lectura, especialmente de las publicaciones.
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
    
    fscanf(fp, "%d", &id);
    fscanf(fp, "%s", username);
    fscanf(fp, "%s", password);
    fgetc(fp);
    fgets(name, sizeof(name), fp);
    name[strcspn(name, "\n")] = '\0';
    fscanf(fp, "%d", &popularity);
    fscanf(fp, "%f", &friendliness);
    fscanf(fp, "%s", category_buf);

    /* carga intereses*/
    int i = 0;
    while (fscanf(fp, "%d", &tempInterests[i].value) == 1) {
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
   
    struct dirent *entry;
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
        fscanf(fp, "%d\n", &post_id);
        fgetc(fp);
        fgets(buffer, sizeof(buffer), fp);
        strptime(buffer, "%Y-%m-%d %H:%M:%S", &post_date);
        fscanf(fp, "%s\n", post_content);
        fclose(fp);
        insert_post(posts, post_content);
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
 * @brief Carga todos los usuarios desde database.
 */
void load_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests){
    DIR *dir = opendir("database");
    if (!dir) {
        perror("ERROR: No se pudo abrir el directorio database");
        return; 
    }

    struct dirent *entry;
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


void load_connections(PtrToHashTable table, Graph graph, GlobalInterests globalInterests){
    DIR *dir = opendir("database");
    if (!dir) {
        return; 
    }
    struct dirent *entry;
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
            fscanf(fp, "%s", username);
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

void load_database(PtrToHashTable table, Graph graph, GlobalInterests globalInterests){
    load_all_users(table, graph, globalInterests);
    load_connections(table, graph, globalInterests);
}

/**
 * @brief Obtiene el usuario de la sesión actual, si existe.
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
    scanf("%255s",username);
    printf("Ingrese su contraseña: ");
    scanf("%255s",password);

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
        return;
    }

    fprintf(file,"%s",username);
    printf("Sesión iniciada correctamente como '%s'.\n", username);
    fclose(file);
}

/**
 * @brief Cierra sesión
 */
void logout(void) {
    if(remove("current.dat")==0){
        print_logo();
        printf(COLOR_RED COLOR_BOLD"Se ha cerrado sesión exitosamente. Nos vemos pronto...\n"COLOR_RESET);
    }
    else {
        printf(COLOR_RED COLOR_BOLD"ERROR: No hay una sesión iniciada\n"COLOR_RESET);
    }
}

/**
 * @brief Registra un nuevo usuario
 */
void register_user(PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    // leer usuario y contraseña

    print_logo();

    char *username=malloc(sizeof(char)*256);
    printf("Ingrese su nombre de usuario: ");
    scanf("%255s",username);
    User user = search_user(username, table);
    while(user) {
        printf(COLOR_RED COLOR_BOLD"ERROR: El nombre de usuario '%s' ya existe. Intente con otro usuario: \n"COLOR_RESET, username);
        scanf("%s",username);
        user = search_user(username, table);
    }

    char *password=malloc(sizeof(char)*256);
    printf("Ingrese su contraseña: ");
    scanf("%255s",password);

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
        scanf("%d",&option);
        if(option<0 || option>=globalInterests.numInterests){
            printf(COLOR_RED COLOR_BOLD"ERROR: ID de interes no válido. Intente nuevamente\n"COLOR_RESET);
        }
        else if(option>0){
            add_interest(user, globalInterests, option);
        }
    } while(option != 0);

    save_user_data(user, globalInterests);
    printf("Usuario registrado correctamente.\n");
}

/**
 * @brief Publicar un post
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
}

/**
 * @brief Seguir a un usuario
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
 */
void unfollow(User user, char* follow, GlobalInterests globalInterests, PtrToHashTable table) {
    User to_unfollow = search_user(follow, table);
    if(!to_unfollow){
        printf("ERROR: '%s' no encontrado.\n", follow);
        return;
    }
    // Llamada a remove_edge
    remove_edge(user, to_unfollow);
    // Guardar datos
    save_user_data(user, globalInterests);
    printf("Has dejado de seguir a '%s'.\n", to_unfollow->username);
}

/**
 * @brief Borrar la cuenta del usuario actual
 */
void delete_account(User user) {
    DIR *dir = opendir("database");
    if (!dir) {
        return;
    }
    closedir(dir);
    logout();
    printf("Se ha borrado la cuenta de '%s' exitosamente.\n", user->username);
}

/**
 * @brief Permite a un usuario editar su información de cuenta
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
        scanf("%d",&option);
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
            scanf("%s",new_username);
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
            scanf("%s",new_password);
            free(user->password);
            user->password = strdup(new_password);
            printf("Se ha modificado su contraseña a '%s'.\n", user->password);
            free(new_password);
            break;
    }
    save_user_data(user, globalInterests);
}

