/**
 * @file heaps.c
 * @brief Funciones para gestionar colas de prioridad
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include "heaps.h"

/**
 * @brief Función para insertar una nueva publicación en el heap
 *
 * @param h Cola de prioridad
 * @param user_name Nombre del usuario
 * @param priority Prioridad de la publicación o de la sugerencia
 * @param content Contenido de la publicación o de la sugerencia
 */
void insert_new_item(heap* h, const char* user_name, double priority, const char* content){
    if (h->size >= MAX_HEAP_SIZE - 1) {
        return;
    }
    post new_post;
    new_post.user_name = strdup(user_name);
    new_post.content = strdup(content);

    // Verificar si strdup falló
    if (new_post.user_name == NULL || new_post.content == NULL) {
        printf("Error al asignar memoria para las cadenas de texto.\n");
        free(new_post.user_name);
        free(new_post.content);
        return;
    }

    new_post.priority = priority;

    // insertar al final y luego hacer heapify_up
    h->posts[h->size] = new_post;
    h->size++;
    heapify_up(h, h->size - 1);
}


/**
 * @brief Función para asegurar que el heap mantenga la propiedad (heapify_up)
 *
 * @param h Cola de prioridad
 * @param index Índice del nodo que debe ajustarse
 */
void heapify_up(heap* h, int index){
    while (index > 0 && h->posts[index].priority > h->posts[(index - 1) / 2].priority) {
        // intercambiar
        post temp = h->posts[index];
        h->posts[index] = h->posts[(index - 1) / 2];
        h->posts[(index - 1) / 2] = temp;

        index = (index - 1) / 2;
    }
}

/**
 * @brief Función para extraer el máximo (el nodo más prioritario) y lo muestra (se utiliza en la función ver_posts)
 *
 * @param h Cola de prioridad
 * @param option Opción para personalizar el formato de salida
 */
void extract_max(heap* h, int option){
    if (h->size == 0) {
        printf("\nno hay más recomendaciones, intente más tarde\n");
        return;
    }

    if (option == 1) printf("%s\n\n|----------\n  %s\n|----------\n", h->posts[0].user_name, h->posts[0].content);
    if (option == 2){
        printf("Hay interes de %f con %s\n", h->posts[0].priority, h->posts[0].user_name);
    }

    // mover el último nodo a la raíz y hacer heapify_down
    h->posts[0] = h->posts[h->size - 1];
    h->size--;
    // restaurar la propiedad del heap
    heapify_down(h, 0);
}

/**
 * @brief Función para extraer el mínimo (el nodo menos prioritario) y mostrarlo.
 *
 * @param h Cola de prioridad
 * @param option Opción para personalizar el formato de salida
 */
void extract_min(heap* h, int option){
    if (h->size == 0) {
        printf("No hay elementos en el heap.\n");
        return;
    }

    // Encontrar el índice del nodo con la menor prioridad
    int minIndex = 0;
    for (int i = 1; i < h->size; i++) {
        if (h->posts[i].priority < h->posts[minIndex].priority) {
            minIndex = i;
        }
    }

    // Mostrar el nodo con menor prioridad
    if (option == 1) {
        printf("%s\n\n|==========================\n  %s\n|==========================\n", h->posts[minIndex].user_name, h->posts[minIndex].content);
    } else if (option == 2) {
        printf("Hay interés de %f con %s\n", h->posts[minIndex].priority, h->posts[minIndex].user_name);
        printf("%s\n\n", h->posts[minIndex].content);
    } else if (option == 3) {
        printf("Hay cercania de %f con %s\n", h->posts[minIndex].priority, h->posts[minIndex].user_name);
    }
    // Mover el último nodo al lugar del mínimo
    h->posts[minIndex] = h->posts[h->size - 1];
    h->size--;

    // Restaurar la propiedad del heap (solo si rompimos la estructura)
    // Se puede optar por ignorar heapify_down aquí, ya que no es un nodo raíz.
}

/**
 * @brief Función para hacer heapify_down (restaurar la propiedad después de extraer el máximo)
 *
 * @param h Cola de prioridad
 * @param index Índice del nodo
 */
void heapify_down(heap* h, int index){
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    if (left < h->size && h->posts[left].priority > h->posts[largest].priority) {
        largest = left;
    }
    if (right < h->size && h->posts[right].priority > h->posts[largest].priority) {
        largest = right;
    }
    if (largest != index) {
        // intercambiar
        post temp = h->posts[index];
        h->posts[index] = h->posts[largest];
        h->posts[largest] = temp;

        heapify_down(h, largest);
    }
}

/**
 * @brief Función para visualizar publicaciones del heap en consola y orden de prioridad
 *
 * @param h cola de prioridad
 */
void watch_posts(heap* h){
    int opcion = 1;

    while (opcion != 2) {
        printf(CLEAN_SCREEN);
        print_logo();
        printf("\n\t\tPUBLICACIONES\n");

        // Verifica si hay publicaciones y extrae la más relevante (máximo)
        switch (opcion) {
            case 1:
                if (h->size > 0) {
                    extract_max(h, 1);  // Extrae y muestra la publicación de mayor prioridad
                } else {
                    printf("\nNo hay más publicaciones para mostrar.\n");
                }
                break;
            case 2:
                break;
            default:
                printf("Opción no válida. Por favor ingrese 1 o 2.\n");
                break;
        }

        printf("\n[1.Continuar][2.Salir]\n");

        // Usar scanf de manera más segura para evitar que el programa se quede atascado
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no válida. Debe ingresar un número.\n");

            while (getchar() != '\n');  // Limpiar el buffer

            opcion = 3;
        }
        // Verificar que la opción sea válida (1 o 2)
        if (opcion != 1 && opcion != 2) {
            printf("Opción no válida. Por favor ingrese 1 o 2.\n");
            opcion = 3; // Forzar repetición del bucle
        }
    }
}

/**
 * @brief función para buscar publicaciones para colocar en el heap
 *
 * @param h cola de prioridad
 * @param table Puntero a la tabla hash
 */
void search_posts_in_my_follows(heap* h, User currentUser){
    if (!currentUser || currentUser->numFollowing == 0) {
        return;
    }

    for (int i = 0; i < currentUser->numFollowing; i++) {
        User u = currentUser->following->next->dest;
        while (u->posts->next != NULL) {
            insert_new_item(h, u->username, 0, u->posts->next->post);
            u->posts = u->posts->next;
        }
        currentUser->following = currentUser->following->next;
    }
}

/**
 * @brief función para buscar publicaciones para colocar en el heap
 *
 * @param h cola de prioridad
 * @param table Puntero a la tabla hash
 */
void search_posts_by_interests(heap* h, PtrToHashTable table, GlobalInterests globalInterestsTable, User currentUser){
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Hashnode *current = table->buckets[i];
        while (current) {
            User u = (User)current->data;
            double jaccard = edge_jaccard(currentUser, u, globalInterestsTable);
            if (jaccard <= 1 && currentUser->username != u->username) {
                while (u->posts->next != NULL) {
                    insert_new_item(h, u->username, jaccard, u->posts->next->post);
                    u->posts = u->posts->next;
                }
            }
            current = current->next;
        }
    }
}

/**
 * @brief función para liberar recursos
 *
 * @param h cola de prioridad
 */
void free_heap(heap* h){
    for (int i = 0; i < h->size; i++) {
        if (h->posts[i].content) free(h->posts[i].content);
        if (h->posts[i].user_name) free(h->posts[i].user_name);
    }
}

/**
 * @brief función para buscar sugerencias de amistades para colocar en el heap
 *
 * @param h cola de prioridad
 * @param table Puntero a la tabla hash
 * @param globalInterestsTable Tabla de intereses globales
 * @param currentUser Usuario actual
 */
void search_new_possible_friends(heap* h, PtrToHashTable table, GlobalInterests globalInterestsTable, User currentUser){
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Hashnode *current = table->buckets[i];
        while (current) {
            User u = (User)current->data;
            double jaccard = edge_jaccard(currentUser, u, globalInterestsTable);
            if (jaccard <= 0.73 && currentUser->username != u->username) {
                int j = 0;
                char interest[1024] = "intereses comunes: \n\t";
                while (j <= globalInterestsTable.numInterests) {
                    if (u->interests[j].value == 1 && currentUser->interests[j].value == 1) {
                        strcat(interest, u->interests[j].name);
                        strcat(interest, " ");
                    }
                    j++;
                }
                insert_new_item(h, u->username, jaccard, interest); // usamos esta función de los posts por que insertar una sugerencia en el heap es igual a insertar un post
            }
            current = current->next;
        }
    }
}

/**
 * @brief función para visualizar sugerencias de amistad en base a la similitud de los usuarios
 *
 * @param h cola de prioridad
 */
void watch_suggestions_friends_of_friends(heap* h){
    printf("\t\tSUGERENCIAS DE AMISTAD DE AMIGOS DE AMIGOS\n\n");
    
    if (h->size == 0) {
        printf("No hay sugerencias de amistad de amigos de amigos.\n\n");
        return;
    }

    while(h->size > 0) {
        extract_min(h, 3); // extraemos el minimo dado que usamos distancia de jaccard, es decir mientras menor número de jaccard, entonces mayor similitud
    }
    printf("\n");
}

/**
 * @brief función para visualizar sugerencias de amistad en base a la similitud de los usuarios
 *
 * @param h cola de prioridad
 */
void watch_suggestions_by_interests(heap* h){
    printf("\t\tSUGERENCIAS DE AMISTAD POR INTERESES\n\n");
    while(h->size > 0) {
        extract_min(h, 2); // extraemos el minimo dado que usamos distancia de jaccard, es decir mientras menor número de jaccard, entonces mayor similitud
    }
}


/**
 * @brief Devuelve índice de un usuario según su posición en el grafo
 *
 * @param graph Grafo
 * @param source Usuario a obtener index
 * @return int
 *
 * @note Se utiliza para el algoritmo \link dijkstra \endlink
 */
int dijkstra_table_index(Graph graph, User source){
    int i = 0;
    GraphList aux = graph->graphUsersList->next;
    while (aux){
        if (aux == source){
            return i;
        }
        aux = aux->next;
        i++;
    }
    printf("Error: No se encontro el usuario en el grafo\n");
    return -1;
}

/**
 * @brief Algoritmo de Dijkstra para caminos cortos, aplicado a la sugerencia de amistades
 * 
 * @param h Heap
 * @param graph Grafo de usuarios
 * @param source Usuario del que se obtendrá los caminos cortos
 */
void dijkstra(heap* h, Graph graph, User source){

    /* Inicialización de la tabla de distancias */
    struct dijkstra_table{
        User user;
        double distance;
        int visited;
    };
    int usersNumber = graph->usersNumber;
    struct dijkstra_table *table = malloc(sizeof(struct dijkstra_table) * usersNumber);
    User user_aux = graph->graphUsersList->next;
    for (int i = 0; i < usersNumber; i++){
        table[i].user = user_aux;
        table[i].distance = INT_MAX;
        table[i].visited = 0;
        user_aux = user_aux->next;
    }

    int sourceIndex = dijkstra_table_index(graph, source);
    table[sourceIndex].user = source;
    table[sourceIndex].distance = 0;

    for (int i = 0; i < usersNumber; i++){
        // Seleccionar el nodo no visitado con la distancia más pequeña
        int u = -1;
        int minDistance = INT_MAX;
        // Buscar el nodo con la distancia más corta
        for (int j = 0; j < usersNumber; j++){
            if (!table[j].visited && table[j].distance < minDistance){
                u = j;
                minDistance = table[j].distance;
            }
        }
        // Si no se encontró ningún nodo accesible, salimos
        if (u == -1)
            break;
        table[u].visited = 1;

        User currentUser = table[u].user;
        // Recorrer los siguiendo del usuario
        Edge edge = currentUser->following->next;
        while (edge != NULL){
            int v = dijkstra_table_index(graph, edge->dest);
            if (table[u].distance + edge->weight < table[v].distance)
            {
                table[v].distance = table[u].distance + edge->weight;
            }
            edge = edge->next;
        }
    }

    // Imprimir distancias
    for (int i = 0; i < usersNumber; i++){
        if (table[i].distance != INT_MAX && source->username != table[i].user->username){
            insert_new_item(h, table[i].user->username, table[i].distance, table[i].user->username);
        }
    }

    free(table);
}