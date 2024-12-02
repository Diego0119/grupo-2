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
User create_new_user(char *username, char *password, char *name, PtrToHashTable table, Graph graph)
{
    if (search_in_hash_table(table, username))
    {
        printf("Error: El nombre de usuario '%s' ya existe\n", username);
        return NULL;
    }

    User user = (User)malloc(sizeof(_User));
    if (!user)
    {
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
UserPosts create_empty_userPosts(void)
{
    UserPosts posts = (UserPosts)malloc(sizeof(PostNode));
    if (!posts)
    {
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
PtrToPostNode insert_post(UserPosts posts, char *content)
{
    PtrToPostNode newPost = (PtrToPostNode)malloc(sizeof(PostNode));
    if (!newPost)
    {
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    time_t t = time(NULL);
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
void delete_userPosts(UserPosts posts)
{
    if (posts->next != NULL)
    {
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
void delete_user(User user, PtrToHashTable table, Graph graph)
{
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
void print_userPosts(UserPosts posts)
{
    if (posts->next == NULL)
    {
        printf("No hay publicaciones\n");
        return;
    }

    PtrToPostNode aux = posts->next;
    while (aux != NULL)
    {
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
void print_user(User user)
{
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
User search_user(char *username, PtrToHashTable table)
{
    return search_in_hash_table(table, username);
}

/**
 * @brief Imprime los seguidores de un usuario
 *
 * @param user Usuario
 */
void print_followers(User user)
{
    Edge aux = user->followers->next;
    printf("Seguidores de %s:\n", user->username);
    while (aux)
    {
        printf("- %s\n", aux->dest->username);
        aux = aux->next;
    }
}

/**
 * @brief Imprime los seguidos de un usuario
 *
 * @param user Usuario
 */
void print_following(User user)
{
    Edge aux = user->following->next;
    printf("Seguidos de %s:\n", user->username);
    while (aux)
    {
        printf("- %s\n", aux->dest->username);
        aux = aux->next;
    }
}

/**
 * @brief Imprime todos los usuarios de la red social
 *
 * @param graph Grafo de usuarios
 */
void print_all_users(Graph graph)
{
    GraphList aux = graph->graphUsersList->next;
    printf("Usuarios (%d):\n", graph->usersNumber);
    while (aux)
    {
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
void free_all_users(PtrToHashTable table, Graph graph)
{
    GraphList aux = graph->graphUsersList->next;
    while (aux)
    {
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
void increment_popularity(User user)
{
    if (user)
        user->popularity++;
}

/**
 * @brief Sugerir usuarios populares basados en popularidad
 *
 * @param table Tabla hash que contiene los usuarios
 */
void suggest_popular_users(HashTable *table)
{
    printf("Usuarios populares:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        Hashnode *current = table->buckets[i];
        while (current)
        {
            User u = (User)current->data;
            if (u->popularity > 10)
            {
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
void sort_posts(User user)
{
    if (!user || !user->posts)
        return;

    int swapped;
    PtrToPostNode ptr1;
    PtrToPostNode lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = user->posts->next;

        while (ptr1 && ptr1->next != lptr)
        {
            if (difftime(mktime(&ptr1->date), mktime(&ptr1->next->date)) < 0)
            {
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

void generate_users(int quantity, PtrToHashTable table, Graph graph)
{
    printf("Creando usuarios, por favor espere...\n");

    for (int i = 0; i <= 3; i++)
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    printf("\n");

    const char *names[] = {
        "Duvan", "Ivan", "Franco", "Diego", "Miguel", "Nicolas", "Jose", "Messi",
        "Carlos", "Juan", "Andres", "Pedro", "Luis", "Raul", "Javier", "Santiago",
        "Ricardo", "Antonio", "Sebastian", "Francisco", "Eduardo", "Julian", "Pablo", "Alfredo", "Oscar",
        "Hector", "Felipe", "Victor", "Martin", "Jorge", "Alberto", "Guillermo", "Mario", "Oscar", "Manuel",
        "Ruben", "Arturo", "Felipe", "Gabriel", "Emilio", "David", "Luis", "Esteban", "Luis", "Raul",
        "Diego", "Pedro", "Ivan", "Ricardo", "Luis", "Hugo", "Alejandro", "Diego", "Alfredo", "Leonardo",
        "Rafael", "Carlos", "Sergio", "Adrian", "Antonio", "Hernan", "Rodrigo", "Erick", "Ernesto", "Victor",
        "Gabriel", "Ariel", "Diego", "Oscar", "Lautaro", "Lucas", "Fabian", "Felipe", "Hector", "Juan",
        "Ramon", "Mauro", "Simón", "Joaquín", "Bautista", "Alfredo", "Luis", "Francisco", "Alberto", "Aureliano",
        "Armando", "Pedro", "Ramon", "Santiago", "Cristian", "Héctor", "Omar", "Ignacio", "Jorge", "Antonio"};

    const char *usernames[] = {
        "BlackWarrior", "TheTerminator", "TheDataStructure", "TheCLanguage", "TheBeast666",
        "TheCryBaby", "ElonMusk", "DonaldTrump", "TechGuru", "CyberKnight", "CodeMaster",
        "ThePhantom", "PixelHunter", "SuperCoder", "NetWarrior", "CodeJunkie", "FutureTech",
        "TheDigitalNomad", "SpaceXplorer", "QuantumCoder", "TheDevKing", "RoboHacker",
        "ByteBeast", "CyberSamurai", "Hackzilla", "CodeSlayer", "PixelPirate", "GameChanger",
        "TechieWarrior", "TheCodeHunter", "DarkCoder", "IronProgrammer", "ByteKnight", "CodeViper",
        "TechAvenger", "ScriptNinja", "DevSlinger", "MatrixManiac", "DigitalDragon", "TechWizard",
        "QuantumWizard", "CodePhantom", "TechGuruX", "ZeroBugHero", "Debugger", "CodeXplorer",
        "CryptoKnight", "DevSorcerer", "AlgorithmMaster", "BinaryHacker", "WebWarlord",
        "CodeSavant", "TheCyberBeast", "NetworkNinja", "TechTitan", "TheCodeSling",
        "BugHunter", "AlgorithmAce", "CodeWarden", "InfinityCoder", "ByteRider",
        "ScriptMaster", "CloudWarrior", "CyberFox", "PixelKnight", "AppMaster",
        "TheDevBeast", "CyberWarriorX", "CodeDemon", "TechSavant", "WebWizard",
        "DevGuruX", "ByteBender", "CloudCoder", "QuantumHacker", "FutureProgrammer",
        "TheTechShaman", "DigitalNomad", "CodeWhisperer", "TechNinja", "BugCrusher",
        "PixelWhisperer", "TheTechEnforcer", "DevWarlord", "CyberViking", "DigitalPhantom",
        "ByteKing", "NetMaster", "DigitalScribe", "CodeWiz", "TechSniper",
        "AlgorithmSleuth", "ZeroBugMaster", "DigitalSorcerer", "CodePioneer", "BugWizard"};

    const char *passwords[] = {"pass1", "pass2", "pass3", "pass4", "pass5", "pass6", "pass7", "pass8"};

    int numNames = sizeof(names) / sizeof(names[0]);
    int numUsernames = sizeof(usernames) / sizeof(usernames[0]);
    int numPasswords = sizeof(passwords) / sizeof(passwords[0]);

    srand(time(NULL));

    for (int i = 0; i < quantity; i++)
    {
        int nameIndex = rand() % numNames;
        int usernameIndex = rand() % numUsernames;
        int passwordIndex = rand() % numPasswords;

        char *name = strdup(names[nameIndex]);
        char *username = strdup(usernames[usernameIndex]);
        char *password = strdup(passwords[passwordIndex]);

        if (search_in_hash_table(table, username))
        {
            printf("Advertencia: El nombre de usuario '%s' ya existe. Saltando...\n", username);
            free(name);
            free(username);
            free(password);
            continue;
        }

        User newUser = create_new_user(username, password, name, table, graph);
        if (!newUser)
        {
            printf("Error al crear el usuario '%s'.\n", username);
            free(name);
            free(username);
            free(password);
            continue;
        }

        printf("Usuario creado: %s (%s)\n", name, username);

        free(name);
        free(username);
        free(password);
    }
}