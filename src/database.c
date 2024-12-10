#include "database.h"

typedef struct _pending_connections {
    char *username;
    char **followers;
    int numFollowers;
    char **following;
    int numFollowing;
    struct _pending_connections *next;
} PendingConnections;

void create_database_dir(void) {
    struct stat st = {0};
    if (stat("database", &st) == -1) {
        if (mkdir("database", 0777) == -1) {
            perror("Error al crear carpeta database");
            exit(EXIT_FAILURE);
        }
    }
}

void save_user_data(User user, GlobalInterests globalInterests) {
    if (!user) return;

    char filename[512];
    snprintf(filename, sizeof(filename), "database/%s_data", user->username);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("No se pudo crear el archivo para el usuario %s\n", user->username);
        return;
    }

    fprintf(fp, "ID: %d\n", user->id);
    fprintf(fp, "Username: %s\n", user->username);
    fprintf(fp, "Password: %s\n", user->password);
    fprintf(fp, "Name: %s\n", user->name);
    fprintf(fp, "Popularity: %d\n", user->popularity);

    // Calcular y guardar amigabilidad y categoría
    float f = calculate_friendliness(user);
    const char *cat = classify_friendliness(f);
    fprintf(fp, "Friendliness: %.2f\n", f);
    fprintf(fp, "Category: %s\n", cat);

    fprintf(fp, "Interests:\n");
    for (int i = 0; i < globalInterests.numInterests; i++) {
        fprintf(fp, "  %s: %d\n", user->interests[i].name, user->interests[i].value);
    }

    fprintf(fp, "Posts:\n");
    PtrToPostNode aux = user->posts->next;
    while (aux) {
        char buffer[128];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &aux->date);
        fprintf(fp, "  PostID: %d\n", aux->id);
        fprintf(fp, "  Fecha: %s\n", buffer);
        fprintf(fp, "  Contenido: %s\n", aux->post);
        fprintf(fp, "  ------------------------\n");
        aux = aux->next;
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

void save_all_users(Graph graph, GlobalInterests globalInterests) {
    create_database_dir();

    PtrToUser currentUser = graph->graphUsersList->next;
    while (currentUser) {
        save_user_data(currentUser, globalInterests);
        currentUser = currentUser->next;
    }

    printf("Se han guardado todos los datos de los usuarios en la carpeta 'database'.\n");
}

void clear_database(void) {
    DIR *dir = opendir("database");
    if (!dir) {
        return;
    }

    struct dirent *entry;
    char path[512];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(path, sizeof(path), "database/%s", entry->d_name);
        remove(path);
    }
    closedir(dir);

    rmdir("database");
    printf("Se han eliminado todos los datos de la base de datos.\n");
}

void confirm_and_cleanup(void) {
    int opcion;
    printf("¿Desea mantener los datos guardados?\n");
    printf("1. Sí, conservar.\n");
    printf("2. No, eliminar todo.\n");
    printf("Elige una opción: ");
    if (scanf("%d", &opcion) != 1) {
        printf("Entrada no válida, se conservarán los datos por defecto.\n");
        return;
    }

    if (opcion == 2) {
        clear_database();
    } else {
        printf("Se conservarán los datos en 'database'.\n");
    }
}

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
    char *end;
    while(*str==' ' || *str=='\t' || *str=='\n') str++;
    if(*str == 0) return;

    end = str + strlen(str) - 1;
    while(end > str && (*end==' ' || *end=='\t' || *end=='\n')) end--;
    *(end+1) = '\0';
}

// load_user_from_file
static PendingConnections* load_user_from_file(const char *filename, PtrToHashTable table, Graph graph, GlobalInterests globalInterests) {
    FILE *fp = fopen(filename,"r");
    if (!fp) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return NULL;
    }

    int id = 0;
    char username[256]={0};
    char password[256]={0};
    char name[256]={0};
    int popularity = 0;
    float friendliness = 0.0f;
    char category_buf[256]={0};

    InterestTable tempInterests = init_user_interests(globalInterests);
    UserPosts posts = create_empty_userPosts();

    char **followers = NULL;
    int allocFollowers = 0;
    int usedFollowers = 0;

    char **following = NULL;
    int allocFollowing = 0;
    int usedFollowing = 0;

    enum State {READ_MAIN, READ_INTERESTS, READ_POSTS, READ_FOLLOWERS, READ_FOLLOWING} state = READ_MAIN;

    char line[512];
    int post_state=0; 
    int post_id=0;
    char post_content[512];
    struct tm post_date;

    while (fgets(line, sizeof(line), fp)) {
        trim(line);
        if (strlen(line)==0) continue;

        if (strncmp(line,"ID:",3)==0) {
            sscanf(line,"ID: %d",&id);
        } else if (strncmp(line,"Username:",9)==0) {
            char *ptr = line+9; while (*ptr==' ') ptr++;
            strncpy(username, ptr, 255);
            username[255]='\0';
            trim(username);
        } else if (strncmp(line,"Password:",9)==0) {
            char *ptr = line+9; while (*ptr==' ') ptr++;
            strncpy(password, ptr, 255);
            password[255]='\0';
            trim(password);
        } else if (strncmp(line,"Name:",5)==0) {
            char *ptr = line+5; while (*ptr==' ') ptr++;
            strncpy(name, ptr, 255);
            name[255]='\0';
            trim(name);
        } else if (strncmp(line,"Popularity:",11)==0) {
            sscanf(line,"Popularity: %d",&popularity);
        } else if (strncmp(line,"Friendliness:",13)==0) {
            sscanf(line,"Friendliness: %f",&friendliness);
        } else if (strncmp(line,"Category:",9)==0) {
            char *ptr = line+9; while(*ptr==' ') ptr++;
            strncpy(category_buf, ptr, 255);
            category_buf[255]='\0';
            trim(category_buf);
        } else if (strncmp(line,"Interests:",10)==0) {
            state = READ_INTERESTS;
        } else if (strncmp(line,"Posts:",6)==0) {
            state = READ_POSTS;
        } else if (strncmp(line,"Followers:",9)==0) {
            state = READ_FOLLOWERS;
        } else if (strncmp(line,"Following:",9)==0) {
            state = READ_FOLLOWING;
        } else {
            if (state == READ_INTERESTS) {
                char iname[256]; int val;
                if (sscanf(line," %255[^:]: %d", iname, &val)==2) {
                    for (int i=0; i<globalInterests.numInterests; i++) {
                        if (strcmp(globalInterests.interestsTable[i], iname)==0) {
                            tempInterests[i].name = globalInterests.interestsTable[i];
                            tempInterests[i].value = val;
                            break;
                        }
                    }
                }
            } else if (state == READ_POSTS) {
                if (strncmp(line,"PostID:",7)==0) {
                    sscanf(line,"PostID: %d",&post_id);
                    post_state=1;
                } else if (strncmp(line,"Fecha:",6)==0) {
                    char date_str[64];
                    sscanf(line,"Fecha: %63[^\n]", date_str);
                    memset(&post_date,0,sizeof(post_date));
                    int year,mon,day,hour,min,sec;
                    if (sscanf(date_str,"%d-%d-%d %d:%d:%d",&year,&mon,&day,&hour,&min,&sec)==6) {
                        post_date.tm_year = year-1900;
                        post_date.tm_mon = mon-1;
                        post_date.tm_mday = day;
                        post_date.tm_hour = hour;
                        post_date.tm_min = min;
                        post_date.tm_sec = sec;
                    }
                    post_state=2;
                } else if (strncmp(line,"Contenido:",10)==0) {
                    char *cptr = line+10;
                    while(*cptr==' ') cptr++;
                    strncpy(post_content, cptr, 511);
                    post_content[511]='\0';
                    post_state=3;
                } else if (strncmp(line,"------------------------",24)==0 && post_state==3) {
                    PtrToPostNode newP = insert_post(posts, post_content, globalInterests);
                    if (newP) {
                        newP->date = post_date;
                    }
                    post_state=0;
                }
            } else if (state == READ_FOLLOWERS) {
                if (strlen(line)>0) {
                    trim(line);
                    if (usedFollowers == allocFollowers) {
                        allocFollowers = allocFollowers==0?4:allocFollowers*2;
                        followers = realloc(followers, allocFollowers*sizeof(char*));
                    }
                    followers[usedFollowers] = strdup(line);
                    usedFollowers++;
                }
            } else if (state == READ_FOLLOWING) {
                if (strlen(line)>0) {
                    trim(line);
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

    printf("Se ha cargado la base de datos con los usuarios existentes.\n");
}