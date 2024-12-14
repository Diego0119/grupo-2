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

/**
 * @brief Crea un nuevo usuario
 *
 * @param username Nombre de usuario
 * @param password Contraseña
 * @param name Nombre completo
 * @note Primero inicializar tabla hash, grafo e intereses globales
 * @return User
 */
User create_new_user(char *username, char *password, char *name, PtrToHashTable table, Graph graph, GlobalInterests globalInterests)
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
    user->interests = init_user_interests(globalInterests);

    // Inicializar amigabilidad y categoría
    user->friendliness = 0.0f;
    user->category = NULL;

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

// Función para generar una publicación aleatoria
char* generate_post(char* buffer, size_t longitud, User user, GlobalInterests globalInterests) {
    const char* acompanamiento[] = {
        "Me gusta el tema: ", "Aveces pienso en ", "Como programador sé hacer ", "Soy el the best developer con respecto a ",
        "Quieres conocerme? soy fan de ", "Lo mejor es ", "Quieres aprender sobre ", "Mi pasión está en ", "Siempre hablo de ",
        "En mis ratos libres disfruto de "
    };

    int acompanamientos = sizeof(acompanamiento) / sizeof(acompanamiento[0]);

    // Elegir un texto de acompañamiento y un interés válido al azar
    const char* acomp = acompanamiento[rand() % acompanamientos];
    int aux = rand() % globalInterests.numInterests;

    while (user->interests[aux].value == 0) {
        aux = rand() % globalInterests.numInterests;
    }

    const char* inter = globalInterests.interestsTable[aux];

    snprintf(buffer, longitud, "%s%s", acomp, inter);

    return buffer;
}

// Función para generar publicaciones aleatorias
UserPosts generate_random_posts(User user, GlobalInterests globalInterests) {
    int numPosts = rand() % 5 + 1; // Generar entre 1 y 10 publicaciones
    char* content = malloc(1024 * sizeof(char));
    if (!content) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numPosts; i++) {
        content = generate_post(content, 1024, user, globalInterests);
        insert_post(user->posts, content); // Copia el contenido
    }

    free(content); // Libera el buffer
    return user->posts;
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
    free_all_edges(user);
    free_user_interests(user->interests);
    if (user->category) free(user->category);
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
        printf("   Fecha: %s", asctime(&aux->date));
        printf("   %s\n", aux->post);
        printf("   ----------------------------------------------------------------\n");
        aux = aux->next;
    }
}

/**
 * @brief Imprime toda la información de un usuario
 *
 * @param user Usuario
 */
void print_user(User user, GlobalInterests globalInterestsTable)
{
    print_logo();
    printf("Nombre: %s\n", user->name);
    printf("Usuario: %s\n", user->username);
    //printf("Contraseña: %s\n", user->password);
    printf("Popularidad (%d) | Seguidores (%d) | Seguidos (%d)\n", user->popularity, user->numFollowers, user->numFollowing);
    // Mostrar amigabilidad y categoría
    printf("Amigabilidad: %.2f\n", user->friendliness);
    printf("Categoría: %s\n", user->category ? user->category : "Desconocida");
    printf("Le gusta: ");
    print_user_interests(user->interests, globalInterestsTable);
    printf("Publicaciones (%d) :\n", user->posts->id);
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
    User u = search_in_hash_table(table, username);
    return u;
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
        printf("- %s, popularidad: %d \n", aux->username, aux->popularity);
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

/* FUNCIONES DE INTERESES */

/**
 * @brief Inicializa la tabla de tópicos globales leyendo el archivo 'subtopics'
 *
 * @return GlobalInterests
 */
GlobalInterests init_global_interests(void)
{
    int numInterests = line_number_in_file("subtopics");

    GlobalInterests globalInterestTable;
    globalInterestTable.interestsTable = (char **)malloc(numInterests * sizeof(char *));
    globalInterestTable.numInterests = numInterests;

    FILE *file_pointer = fopen("subtopics", "r");
    if (!file_pointer)
    {
        printf("ERROR: No se pudo abrir el archivo 'subtopics'. Por favor asegurese que este archivo existe y esté en el mismo directorio que el programa.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numInterests; i++)
    {
        globalInterestTable.interestsTable[i] = (char *)malloc(sizeof(char) * numInterests);
        if (fgets(globalInterestTable.interestsTable[i], MAX_CHAR, file_pointer) == NULL) exit(EXIT_FAILURE);
        globalInterestTable.interestsTable[i][strlen(globalInterestTable.interestsTable[i]) - 1] = '\0';
    }
    fclose(file_pointer);
    return globalInterestTable;
}

/**
 * @brief Libera la lista de intereses globales
 *
 * @param globalInterestTable
 */

void free_global_interests(GlobalInterests globalInterestTable)
{
    for (int i = 0; i < globalInterestTable.numInterests; i++)
    {
        free(globalInterestTable.interestsTable[i]);
    }
    free(globalInterestTable.interestsTable);
}

/**
 * @brief Inicializa los intereses de un usuario
 *
 * @param globalInterests Lista de intereses globales
 * @return InterestTable
 */

InterestTable init_user_interests(GlobalInterests globalInterestTable)
{
    InterestTable userInterests = (InterestTable)malloc(globalInterestTable.numInterests * sizeof(Interest));

    for (int i = 0; i < globalInterestTable.numInterests; i++)
    {
        userInterests[i].value = 0;
        userInterests[i].globalId = i;
        userInterests[i].name = globalInterestTable.interestsTable[i];
    }
    return userInterests;
}


/**
 * @brief Libera memoria los intereses de un usuario
 *
 * @param userInterests
 */
void free_user_interests(InterestTable userInterests)
{
    free(userInterests);
}

/**
 * @brief Imprime los intereses de un usuario
 *
 * @param userInterests
 * @param globalInterestTable
 */
void print_user_interests(InterestTable userInterests, GlobalInterests globalInterestTable)
{
    for (int i = 0; i < globalInterestTable.numInterests; i++)
    {
        if(userInterests[i].value==1){
            printf("%s ", userInterests[i].name);
        }
    }
    printf("\n");
}

/**
 * @brief Calcula la DIFERENCIA de jaccard entre dos usuarios
 *
 * @param user1 Usuario 1
 * @param user2 Usuario 2
 * @param globalInterestTable Lista de intereses globales
 * @return double
 * @note Utilizarse en el peso de la conexion
 */
double edge_jaccard(User user1, User user2, GlobalInterests globalInterestTable){
    double jaccard;

    int diff = 0, same = 0;

    for (int i = 0; i < globalInterestTable.numInterests; i++)
    {
        if (user1->interests[i].value == 1 && user2->interests[i].value == 1)
        {
            same++;
        }
        if (user1->interests[i].value == 0 && user2->interests[i].value == 1)
        {
            diff++;
        }
        if (user1->interests[i].value == 1 && user2->interests[i].value == 0)
        {
            diff++;
        }
    }

    jaccard = (double)same / (same + diff);

    return 1 - jaccard;
}

/**
 * @brief Genera usuarios aleatorios
 *
 * @param quantity Cantidad de usuarios a generar
 * @param table Tabla hash que contiene los usuarios
 * @param graph El grafo de los usuarios
 */
void generate_users(int quantity, PtrToHashTable table, Graph graph, GlobalInterests globalInterests){
    if(database_exists_and_not_empty()!=0){
        printf("Ya existe una base de datos. Ejecute './devgraph.out -c' para borrarla.\n");
        free_structures_and_exit(table, graph, globalInterests);
    }

    printf("Creando usuarios, por favor espere...\n");

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
        "CodeSavant", "TheCyberBeast", "MiguelLoaizaMachuca(ElPhantom)", "NetworkNinja", "TechTitan", "TheCodeSling",
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
    srand(time(0)^clock());
    for (int i = 0; i < quantity; i++)
    {
        int nameIndex = rand() % numNames;
        int usernameIndex = rand() % numUsernames;
        int passwordIndex = rand() % numPasswords;

        char *name = strdup(names[nameIndex]);
        //char *username = strdup(usernames[usernameIndex]);
        char username[50];
        int num = rand() % 1000;
        snprintf(username, sizeof(username), "%s%d", usernames[usernameIndex], num);
        char *password = strdup(passwords[passwordIndex]);

        if (search_in_hash_table(table, username))
        {
            //printf("Advertencia: El nombre de usuario '%s' ya existe. Generando otro usuario...\n", username);
            free(name);
            free(password);
            i--;
            continue;
        }

        User newUser = create_new_user(username, password, name, table, graph, globalInterests);
        if (!newUser)
        {
            //printf("Error al crear el usuario '%s'.\n", username);
            free(name);
            free(password);
            continue;
        }
        for(int i=0; i<rand()%globalInterests.numInterests; i++){
            add_interest(newUser, globalInterests, rand()% globalInterests.numInterests);
        }
        printf("%d. Usuario creado: %s (%s)\n", i, name, username);

        free(name);
        free(password);

    }
}

/**
 * @brief Genera conexiones aleatorias entre usuarios.
 *
 * @param quantity Cantidad de usuarios en el grafo.
 * @param graph El grafo de usuarios.
 */
void generate_random_connections(Graph graph, GlobalInterests globalInterests)
{
    PtrToUser currentUser = graph->graphUsersList;

    for (int i = 0; i < graph->usersNumber; i++)
    {
        if (currentUser == NULL)
        {
            printf("La lista de usuarios está vacía.\n");
            return;
        }

        int randomIndex = rand() % graph->usersNumber;

        currentUser = graph->graphUsersList->next;

        for (int i = 0; i < randomIndex; i++)
        {
            if (currentUser->next != NULL)
            {
                currentUser = currentUser->next;
            }
            else
            {
                printf("Error: la lista es más corta de lo esperado.\n");
                return;
            }
        }

        //printf("Usuario seleccionado en la iteracion %d: %s\n", i, currentUser->username);

        User randomUser = graph->graphUsersList->next;
        int random_index = rand() % graph->usersNumber;

        for (int j = 0; j < random_index; j++)
        {
            randomUser = randomUser->next;
        }

        if (randomUser != currentUser)
        {
            add_edge(currentUser, randomUser, globalInterests);
            //printf("Conectando el usuario '%s' con '%s'.\n", currentUser->username, randomUser->username);
        }
        else
        {
            //printf("No se puede conectar el usuario '%s' consigo mismo.\n", currentUser->username);
        }
    }
}

/**
 * @brief Calcula qué tan amigable es un usuario.
 *
 * @param user Puntero al usuario a evaluar.
 * @return Un valor flotante que representa la amigabilidad del usuario.
 *         Valores más altos indican mayor amigabilidad.
 */

float calculate_friendliness(User user)
{
    if (user == NULL)
    {
        return -1.0f;
    }

    if (user->numFollowing == 0 && user->numFollowers == 0)
    {
        return 0.0f;
    }

    float friendliness = 0.0f;
    if (user->numFollowing > 0)
    {
        friendliness = ((float)user->numFollowers / (float)user->numFollowing) + (float)user->numFollowers;
    }
    else
    {
        friendliness = (float)user->numFollowers;
    }

    return friendliness;
}

/**
 * @brief Imprime una tabla de amigabilidad para todos los usuarios en el grafo.
 *
 * @param graph Grafo que contiene la lista de usuarios a evaluar.
 */

void print_friendliness_table(Graph graph)
{
    printf("=============================================================\n");
    printf("| %-20s | %-15s | %-20s |\n", "Usuario", "Amigabilidad", "Categoría");
    printf("=============================================================\n");

    PtrToUser currentUser = graph->graphUsersList;
    while (currentUser != NULL)
    {
        float friendliness = currentUser->friendliness;
        const char *category = currentUser->category ? currentUser->category : classify_friendliness(friendliness);
        printf("| %-20s | %-15.2f | %-20s |\n", currentUser->username ? currentUser->username : "(null)", friendliness, category);
        currentUser = currentUser->next;
    }

    printf("=============================================================\n");
}

/**
 * @brief Clasifica el nivel de amigabilidad de un usuario según su puntaje.
 *
 *
 * @param friendliness Puntaje de amigabilidad calculado.
 * @return const char* Categoría textual que describe el nivel de amigabilidad.
 */
const char *classify_friendliness(float friendliness)
{
    if (friendliness <= 0)
    {
        return "Nada amigable";
    }
    else if (friendliness <= 1)
    {
        return "Poco amigable";
    }
    else if (friendliness <= 2)
    {
        return "Amigable";
    }
    else
    {
        return "Muy amigable";
    }
}

void add_interest(User user, GlobalInterests globalInterestTable, int interestId){
    if(interestId>=globalInterestTable.numInterests){
        printf("ERROR: Id de interés inválido\n");
        return;
    }
    user->interests[interestId].value = 1;
}

void print_global_interests(GlobalInterests globalInterestTable){
    for(int i=1; i<globalInterestTable.numInterests-1; i++){
        printf("- %d. %s\n", i, globalInterestTable.interestsTable[i]);
    }
}
