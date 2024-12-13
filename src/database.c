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
/*void save_user_data(User user, GlobalInterests globalInterests) {
    struct stat st = {0};

    // crear carpeta para el usuario, si no existe
    char filename[512];
    snprintf(filename, sizeof(filename), "database/%s_data", user->username);
    if (stat(filename, &st) == -1) {
        if (mkdir(filename, 0777) == -1) {
            printf("Error al crear directorio para el usuario '%s'\n", user->username);
            exit(EXIT_FAILURE);
        }
    }
    
    // crear o escribir los archivos de datos
    /* data 
    FILE *fp;
    snprintf(filename, sizeof(filename), "database/%s_data/data.dat", user->username);
    fp = fopen(filename,"w");
    fprintf(fp, "ID: %d\n", user->id);
    fprintf(fp, "Username: %s\n", user->username);
    fprintf(fp, "Password: %s\n", user->password);
    fprintf(fp, "Name: %s\n", user->name);
    fprintf(fp, "Popularity: %d\n", user->popularity);
    float f = calculate_friendliness(user);
    const char *cat = classify_friendliness(f);
    fprintf(fp, "Friendliness: %.2f\n", f);
    fprintf(fp, "Category: %s\n", cat);
    fprintf(fp, "Interests:\n");
    for (int i = 0; i < globalInterests.numInterests; i++) {
        fprintf(fp, "  %s: %d\n", user->interests[i].name, user->interests[i].value);
    }
    fclose(fp);

    /*seguidores
    snprintf(filename, sizeof(filename), "database/%s_data/followers.dat", user->username);
    fp = fopen(filename,"w");
    Edge e = user->followers->next;
    while(e){
        fprintf(fp, "  %s\n", e->dest->username);
        e = e->next;
    }


}*/


void save_user_data(User user, GlobalInterests globalInterests) {

    char filename[512];
    snprintf(filename, sizeof(filename), "database/%s_data", user->username);

    FILE *fp = fopen(filename, "w");
    if(!fp){
        return;
    }

    fprintf(fp, "ID: %d\n", user->id);
    fprintf(fp, "Username: %s\n", user->username);
    fprintf(fp, "Password: %s\n", user->password);
    fprintf(fp, "Name: %s\n", user->name);
    fprintf(fp, "Popularity: %d\n", user->popularity);

    // Amigabilidad y categoría
    float f = calculate_friendliness(user);
    const char *cat = classify_friendliness(f);
    fprintf(fp, "Friendliness: %.2f\n", f);
    fprintf(fp, "Category: %s\n", cat);

    fprintf(fp, "Interests:\n");
    for (int i = 0; i < globalInterests.numInterests; i++) {
        fprintf(fp, "  %s: %d\n", user->interests[i].name, user->interests[i].value);
    }

    fprintf(fp, "Posts:\n");
    PtrToPostNode postAux = user->posts->next;
    while (postAux) {
        char buffer[128];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &postAux->date);
        fprintf(fp, "  PostID: %d\n", postAux->id);
        fprintf(fp, "  Fecha: %s\n", buffer);
        fprintf(fp, "  Contenido: %s\n", postAux->post);
        fprintf(fp, "  ------------------------\n");
        postAux = postAux->next;
    }

    fprintf(fp, "Followers:\n");
    Edge e = user->followers->next;
    while (e) {
        fprintf(fp, "  %s\n", e->dest->username);
        e = e->next;
    }

    fprintf(fp, "Following:\n");
    e = user->following->next;
    while (e) {
        fprintf(fp, "  %s\n", e->dest->username);
        e = e->next;
    }

    fclose(fp);
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
        snprintf(path, sizeof(path), "database/%s", entry->d_name);
        remove(path);
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

/* Estado para parsing de publicaciones */
typedef enum {
    POST_WAIT_ID,
    POST_WAIT_FECHA,
    POST_WAIT_CONTENIDO,
    POST_READY
} PostState;

/**
 * @brief Carga un usuario desde un archivo y devuelve su PendingConnections.
 * 
 * Se añade debug extra para entender el flujo de lectura, especialmente de las publicaciones.
 */
static PendingConnections* load_user_from_file(const char *filename, PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    FILE *fp = fopen(filename,"r");
    if (!fp) {
        return NULL;
    }

    int id = 0;
    char username[256]="";
    char password[256]="";
    char name[256]="";
    int popularity = 0;
    float friendliness = 0.0f;
    char category_buf[256]="";

    InterestTable tempInterests = init_user_interests(globalInterests);
    UserPosts posts = create_empty_userPosts();

    char **followers = NULL;
    int allocFollowers = 0, usedFollowers = 0;

    char **following = NULL;
    int allocFollowing = 0, usedFollowing = 0;

    enum State {READ_MAIN, READ_INTERESTS, READ_POSTS, READ_FOLLOWERS, READ_FOLLOWING} state = READ_MAIN;

    char line[512];
    PostState post_state = POST_WAIT_ID;
    int post_id = 0;
    char post_content[512]="";
    struct tm post_date; 
    memset(&post_date, 0, sizeof(post_date));

    while (fgets(line, sizeof(line), fp)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strncmp(line,"ID:",3)==0) {
            sscanf(line,"ID: %d",&id);
        } else if (strncmp(line,"Username:",9)==0) {
            char *ptr = line+9; while (*ptr==' ') ptr++;
            strncpy(username, ptr, 255); username[255]='\0';
            trim(username);
        } else if (strncmp(line,"Password:",9)==0) {
            char *ptr = line+9; while (*ptr==' ') ptr++;
            strncpy(password, ptr, 255); password[255]='\0';
            trim(password);
        } else if (strncmp(line,"Name:",5)==0) {
            char *ptr = line+5; while (*ptr==' ') ptr++;
            strncpy(name, ptr, 255); name[255]='\0';
            trim(name);
        } else if (strncmp(line,"Popularity:",11)==0) {
            sscanf(line,"Popularity: %d",&popularity);
        } else if (strncmp(line,"Friendliness:",13)==0) {
            sscanf(line,"Friendliness: %f",&friendliness);
        } else if (strncmp(line,"Category:",9)==0) {
            char *ptr = line+9; while(*ptr==' ') ptr++;
            strncpy(category_buf, ptr, 255); category_buf[255]='\0';
            trim(category_buf);
        } else if (strncmp(line,"Interests:",10)==0) {
            state = READ_INTERESTS;
        } else if (strncmp(line,"Posts:",6)==0) {
            state = READ_POSTS;
            post_state = POST_WAIT_ID;
        } else if (strncmp(line,"Followers:",9)==0) {
            state = READ_FOLLOWERS;
        } else if (strncmp(line,"Following:",9)==0) {
            state = READ_FOLLOWING;
        } else {
            if (state == READ_INTERESTS) {
                char iname[256]; int val;
                if (sscanf(line,"%255[^:]: %d", iname, &val)==2) {
                    trim(iname);
                    for (int i=0; i<globalInterests.numInterests; i++) {
                        if (strcmp(globalInterests.interestsTable[i], iname)==0) {
                            tempInterests[i].name = globalInterests.interestsTable[i];
                            tempInterests[i].value = val;
                            break;
                        }
                    }
                }
            } else if (state == READ_POSTS) {
                // Proceso de posts
                if (strncmp(line,"PostID:",7)==0) {
                    if (post_state != POST_WAIT_ID) {
                        post_state = POST_WAIT_ID;
                    }
                    sscanf(line,"PostID: %d",&post_id);
                    post_state = POST_WAIT_FECHA;
                } else if (strncmp(line,"Fecha:",6)==0) {
                    if (post_state != POST_WAIT_FECHA) {
                        post_state = POST_WAIT_ID;
                        continue;
                    }
                    char date_str[64];
                    if (sscanf(line,"Fecha: %63[^\n]", date_str)==1) {
                        int year,mon,day,hour,min,sec;
                        if (sscanf(date_str,"%d-%d-%d %d:%d:%d",&year,&mon,&day,&hour,&min,&sec)==6) {
                            post_date.tm_year = year-1900;
                            post_date.tm_mon = mon-1;
                            post_date.tm_mday = day;
                            post_date.tm_hour = hour;
                            post_date.tm_min = min;
                            post_date.tm_sec = sec;
                            post_state = POST_WAIT_CONTENIDO;
                        } else {
                            post_state = POST_WAIT_ID;
                        }
                    }
                } else if (strncmp(line,"Contenido:",10)==0) {
                    if (post_state != POST_WAIT_CONTENIDO) {
                        post_state = POST_WAIT_ID;
                        continue;
                    }
                    char *cptr = line+10;
                    while(*cptr==' ') cptr++;
                    strncpy(post_content, cptr, 511);
                    post_content[511]='\0';
                    trim(post_content);
                    post_state = POST_READY;
                } else if (strncmp(line,"------------------------",24)==0) {
                    if (post_state == POST_READY) {
                        // Insertar el post
                        PtrToPostNode newP = insert_post(posts, post_content);
                        if (newP) {
                            newP->id = post_id;
                            newP->date = post_date;
                        } else {
                        }
                    }
                    // Reiniciar estado para el siguiente post
                    post_state = POST_WAIT_ID;
                    memset(post_content,0,sizeof(post_content));
                }
            } else if (state == READ_FOLLOWERS) {
                // Lectura de seguidores
                if (strlen(line)>0) {
                    if (usedFollowers == allocFollowers) {
                        allocFollowers = allocFollowers==0?4:allocFollowers*2;
                        followers = realloc(followers, allocFollowers*sizeof(char*));
                    }
                    followers[usedFollowers] = strdup(line);
                    usedFollowers++;
                }
            } else if (state == READ_FOLLOWING) {
                // Lectura de seguidos
                if (strlen(line)>0) {
                    if (usedFollowing == allocFollowing) {
                        allocFollowing = allocFollowing==0?4:allocFollowing*2;
                        following = realloc(following, allocFollowing*sizeof(char*));
                    }
                    following[usedFollowing] = strdup(line);
                    usedFollowing++;
                }
            }
        }
    }

    fclose(fp);

    // Crear el usuario real en memoria
    User newUser = create_new_user(username, password, name, table, graph, globalInterests);
    if (!newUser) {
        free(tempInterests);
        for (int i=0; i<usedFollowers; i++) free(followers[i]);
        free(followers);
        for (int i=0; i<usedFollowing; i++) free(following[i]);
        free(following);
        delete_userPosts(posts);
        return NULL;
    }

    // Asignar intereses y posts al nuevo usuario
    free_user_interests(newUser->interests);
    newUser->interests = tempInterests;

    delete_userPosts(newUser->posts);
    newUser->posts = posts;
    newUser->popularity = popularity;

    // Asignar amigabilidad y categoría
    newUser->friendliness = friendliness;
    newUser->category = strdup(category_buf);

    PendingConnections *pc = malloc(sizeof(PendingConnections));
    pc->username = strdup(username);
    pc->followers = followers;
    pc->numFollowers = usedFollowers;
    pc->following = following;
    pc->numFollowing = usedFollowing;
    pc->next = NULL;


    return pc;
}

/**
 * @brief Carga todos los usuarios desde database.
 */
void load_all_users(PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    DIR *dir = opendir("database");
    if (!dir) {
        return; 
    }

    PendingConnections *head = NULL;
    PendingConnections *tail = NULL;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0) continue;
        char *ext = strstr(entry->d_name,"_data");
        if (!ext) continue;

        char path[512];
        snprintf(path,sizeof(path),"database/%s",entry->d_name);
        PendingConnections *pc = load_user_from_file(path, table, graph, globalInterests);
        if (pc) {
            if (!head) {
                head = pc;
                tail = pc;
            } else {
                tail->next = pc;
                tail = pc;
            }
        }
    }
    closedir(dir);

    PendingConnections *current = head;
    while (current) {
        User mainUser = search_user(current->username, table);
        if (mainUser) {
            for (int i=0; i<current->numFollowers; i++) {
                User followerUser = search_user(current->followers[i], table);
                if (followerUser && followerUser!=mainUser) {
                    add_edge(followerUser, mainUser, globalInterests);
                }
            }

            for (int i=0; i<current->numFollowing; i++) {
                User followingUser = search_user(current->following[i], table);
                if (followingUser && followingUser!=mainUser) {
                    add_edge(mainUser, followingUser, globalInterests);
                }
            }
        }
        current = current->next;
    }

    current = head;
    while (current) {
        PendingConnections *next = current->next;
        free(current->username);
        for (int i=0; i<current->numFollowers; i++) free(current->followers[i]);
        free(current->followers);
        for (int i=0; i<current->numFollowing; i++) free(current->following[i]);
        free(current->following);
        free(current);
        current = next;
    }

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

    char username[256];
    char password[256];
    printf("Ingrese su nombre de usuario: ");
    scanf("%255s",username);
    printf("Ingrese su contraseña: ");
    scanf("%255s",password);

    User user = search_user(username, graph);
    if (!user) {
        printf("ERROR: Usuario no existe.\n");
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
    if (remove("current.dat") == 0) {
        printf("Sesión cerrada.\n");
    }
}

/**
 * @brief Registra un nuevo usuario
 */
void register_user(PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    char username[256], password[256], name[256];

    printf("Ingrese su nombre de usuario: ");
    scanf("%255s",username);
    User user = search_user(username, table);
    while(user) {
        printf("ERROR: '%s' ya existe. Intente otro: ", username);
        scanf("%255s",username);
        user = search_user(username, table);
    }

    printf("Ingrese su contraseña: ");
    scanf("%255s",password);

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer

    printf("Ingrese su nombre: ");
    if (fgets(name, 256, stdin) != NULL) {
        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        }
    }

    user = create_new_user(username, password, name, table, graph, globalInterests);
    if(!user){
        printf("ERROR: No se pudo crear '%s'.\n", username);
        return;
    }

    printf("Bienvenido/a %s a DevGraph! Ingrese ID de intereses (0 para terminar)\n", user->name);
    print_global_interests(globalInterests);
    int option;
    do {
        scanf("%d",&option);
        if(option<0 || option>=globalInterests.numInterests){
            printf("ID no válido.\n");
        }
        else if(option>0){
            add_interest(user, globalInterests, option);
        }
    } while(option>0);

    save_user_data(user, globalInterests);
    printf("Usuario registrado correctamente.\n");
}

/**
 * @brief Publicar un post
 */
void write_post(User user, GlobalInterests globalInterests) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Publicando como '%s'. Ingrese contenido:\n", user->username);
    char content[512];
    if (fgets(content, 512, stdin) != NULL) {
        size_t len = strlen(content);
        if (len > 0 && content[len-1] == '\n') {
            content[len-1] = '\0';
        }
    }

    PtrToPostNode newP = insert_post(user->posts, content);
    if (newP) {
        printf("Publicación creada.\n");
    }
    save_user_data(user, globalInterests);
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

