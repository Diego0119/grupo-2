/**
 * @file graph.c
 * @brief Implementación de funciones para gestionar grafos y relaciones entre usuarios
 * @authors
 * - Iván Mansilla
 * - Franco Aguilar
 * - Diego Sanhueza
 * - Duvan Figueroa
 * - Nicolás Álvarez
 * - Miguel Maripillan
 */

#include "graph.h"

/**
 * @brief Inicia una lista de adyacencia vacía
 *
 * @return Edge
 */
Edge init_empty_edge(void)
{
    Edge newEdge = (Edge)malloc(sizeof(struct _edge));
    if (!newEdge)
    {
        printf("ERROR: No hay memoria suficiente\n");
        exit(EXIT_FAILURE);
    }
    newEdge->dest = NULL;
    newEdge->weight = 0;
    newEdge->next = NULL;
    return newEdge;
}

/**
 * @brief Busca un usuario dentro de una lista de adyacencia
 *
 * @param edge Lista de adyacencia
 * @param user Usuario a buscar
 * @return Edge Adyacencia encontrada o NULL si no se encuentra
 */
Edge search_user_in_edge(Edge edge, User user)
{
    Edge aux = edge;
    while (aux->dest != user)
    {
        if (!aux->next)
        {
            return NULL;
        }
        aux = aux->next;
    }
    return aux;
}

/**
 * @brief Busca la adyacencia anterior a un usuario en una lista de adyacencia
 *
 * @param edge Lista de adyacencia
 * @param user Usuario a buscar la adyacencia anterior
 * @return Edge Adyacencia anterior encontrada o NULL si no se encuentra
 */
Edge search_previous_in_edge(Edge edge, User user){
    
    if (!edge || !user){
        return NULL;
    }

    Edge aux = edge;
    while (aux->next)
    {
        if (aux->next->dest == user)
        {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

/**
 * @brief Inicializa un grafo vacío
 *
 * @return Graph
 */
Graph initialize_graph(void)
{
    Graph newGraph = (Graph)malloc(sizeof(struct _graph));
    if (!newGraph)
    {
        printf("Error al crear el grafo\n");
        return NULL;
    }
    newGraph->graphUsersList = (GraphList)malloc(sizeof(struct _user));
    if (!newGraph->graphUsersList)
    {
        printf("Error al crear el grafo (lista de usuarios)\n");
        free_graph(newGraph);
        return NULL;
    }
    newGraph->graphUsersList->next = NULL;

    newGraph->usersNumber = 0;
    return newGraph;
}

/**
 * @brief Libera la memoria de un grafo
 *
 * @param graph Grafo a liberar
 *
 * @note Ejecutar despues de @see free_all_users
 */
void free_graph(Graph graph)
{
    free(graph->graphUsersList);
    free(graph);
}

/**
 * @brief Añade un usuario (ya creado) al grafo
 *
 * @param graph Grafo
 * @param user Usuario a añadir
 */
void add_user_to_graph(Graph graph, User user)
{
    user->next = graph->graphUsersList->next;
    graph->graphUsersList->next = user;
    graph->usersNumber++;
}

/**
 * @brief Elimina un usuario del grafo
 *
 * @param graph Grafo
 * @param user Usuario a eliminar
 *
 * @note No elimina al usuario en si, lo saca del grafo y libera sus conexiones
 */
void remove_user_from_graph(Graph graph, User user)
{
    GraphList aux = graph->graphUsersList;
    while (aux->next != user)
    {
        aux = aux->next;
    }
    aux->next = user->next;
    user->next = NULL;
    free_all_edges(user);
    graph->usersNumber--;
}

/**
 * @brief Añade una adyacencia entre dos usuarios
 *
 * @param user1 Usuario 1 que añade a Usuario 2
 * @param user2 Usuario 2 será añadido por usuario 1
 * @param weigth Peso de la conexion
 */
void add_edge(User user1, User user2, GlobalInterests globalInterests)
{

    if (user1 == user2)
    {
        printf("Un usuario no puede ser amigo de si mismo\n");
        exit(EXIT_FAILURE);
    }
    
    double weight=edge_jaccard(user1, user2, globalInterests);
    
    Edge newEdgeUser1 = (Edge)malloc(sizeof(struct _edge));
    newEdgeUser1->dest = user2;
    newEdgeUser1->weight = weight;

    Edge newEdgeUser2 = (Edge)malloc(sizeof(struct _edge));
    newEdgeUser2->dest = user1;
    newEdgeUser2->weight = weight;

    if (!newEdgeUser1 || !newEdgeUser2)
    {
        printf("Error al crear enlace entre usuarios\n");
        return;
    }

    /* unir conexiones*/
    newEdgeUser1->next = user1->following->next;
    user1->following->next = newEdgeUser1;
    user1->numFollowing++;

    newEdgeUser2->next = user2->followers->next;
    user2->followers->next = newEdgeUser2;
    user2->numFollowers++;
}

/**
 * @brief Permite que un usuario deje de seguir a otro
 *
 * @param user1 Usuario 1 que deja de seguir a Usuario 2
 * @param user2 Usuario 2 le deja de seguir Usuario 1
 *
 * @note Para usuario 1 elimina a usuario 1 de su lista de seguidos, y para usuario 2 se elimina usuario de su lista de seguidores
 */
void remove_edge(User user1, User user2)
{   
        if (!user1 || !user2) {
        return;
    }
    if (!user1->following || !user2->followers) {
        return;
    }


    Edge aux1 = search_previous_in_edge(user1->following, user2);
    Edge aux2 = search_previous_in_edge(user2->followers, user1);
    if (!aux1 || !aux2){
        return;
    }

    Edge toRemove1 = aux1->next;
    Edge toRemove2 = aux2->next;
    
    if (toRemove1)
    {
        aux1->next = toRemove1->next;
        free(toRemove1);
        user1->numFollowing--;
    }

    if (toRemove2)
    {
        aux2->next = toRemove2->next;
        free(toRemove2);
        user2->numFollowers--;
    }
}

/**
 * @brief Libera todas las adyacencias de un usuario
 *
 * @param user Usuario
 *
 * @note Libera también las adyacencias de los usuarios que lo siguen o sigue
 */
void free_all_edges(User user)
{
    // Liberar todas las conexiones de la lista de seguidos
    Edge current = user->following->next;
    while (current)
    {
        Edge next = current->next;
        remove_edge(user, current->dest);
        current = next;
    }
    user->numFollowing = 0;

    // Liberar todas las conexiones de la lista de seguidores
    current = user->followers->next;
    while (current)
    {
        Edge next = current->next;
        remove_edge(current->dest, user);
        current = next;
    }
    user->numFollowers = 0;
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
int dijkstra_table_index(Graph graph, User source)
{
    int i = 0;
    GraphList aux = graph->graphUsersList->next;
    while (aux)
    {
        if (aux == source)
        {
            return i;
            break;
        }
        aux = aux->next;
        i++;
    }
    printf("Error: No se encontro el usuario en el grafo\n");
    return -1;
}

/**
 * @brief Algoritmo de Dijkstra
 *
 * @param graph Grafo
 * @param source Usuario fuente
 *
 * @note Solo devuelve distancias entre las adyacencias de seguidos
 */
void dijkstra(Graph graph, User source)
{

    /* Inicialización de la tabla de distancias */
    struct dijkstra_table
    {
        User user;
        double distance;
        int visited;
    };
    int usersNumber = graph->usersNumber;
    struct dijkstra_table *table = malloc(sizeof(struct dijkstra_table) * usersNumber);
    User user_aux = graph->graphUsersList->next;
    for (int i = 0; i < usersNumber; i++)
    {
        table[i].user = user_aux;
        table[i].distance = INT_MAX;
        table[i].visited = 0;
        user_aux = user_aux->next;
    }

    int sourceIndex = dijkstra_table_index(graph, source);
    table[sourceIndex].user = source;
    table[sourceIndex].distance = 0;

    for (int i = 0; i < usersNumber; i++)
    {
        // Seleccionar el nodo no visitado con la distancia más pequeña
        int u = -1;
        int minDistance = INT_MAX;
        // Buscar el nodo con la distancia más corta
        for (int j = 0; j < usersNumber; j++)
        {
            if (!table[j].visited && table[j].distance < minDistance)
            {
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
        while (edge != NULL)
        {
            int v = dijkstra_table_index(graph, edge->dest);
            if (table[u].distance + edge->weight < table[v].distance)
            {
                table[v].distance = table[u].distance + edge->weight;
            }
            edge = edge->next;
        }
    }

    // Imprimir distancias
    for (int i = 0; i < usersNumber; i++)
    {
        if (table[i].distance == INT_MAX)
        {
            printf("Distancia desde %s a %s: Inalcanzable\n", source->username, table[i].user->username);
        }
        else
        {
            printf("Distancia desde %s a %s: %.2f\n", source->username, table[i].user->username, table[i].distance);
        }
    }

    free(table);
}

void BFS(Graph graph, User startUser)
{
    if (!graph || !startUser)
    {
        printf("Error: Grafo o usuario inicial inválido.\n");
        return;
    }

    int MAX_USERS = graph->usersNumber;

    User *queue=malloc(sizeof(User)*MAX_USERS);
    int *visited=malloc(sizeof(int)*MAX_USERS);
    for(int i=0;i<MAX_USERS;i++){
        queue[i]=NULL;
        visited[i]=0;
    }
    int front = 0, rear = 0;

    int startIndex = dijkstra_table_index(graph, startUser);
    if (startIndex == -1)
        return;

    visited[startIndex] = 1;
    queue[rear++] = startUser;

    printf("Recorrido BFS desde %s:\n", startUser->username);

    while (front < rear)
    {
        User currentUser = queue[front++];
        printf("- %s\n", currentUser->username);

        Edge edge = currentUser->following->next;
        while (edge)
        {
            int neighborIndex = dijkstra_table_index(graph, edge->dest);
            if (!visited[neighborIndex])
            {
                visited[neighborIndex] = 1;
                queue[rear++] = edge->dest;
            }
            edge = edge->next;
        }
    }
    free(visited);
    free(queue);
}

void DFS(Graph graph, User startUser)
{
    if (!graph || !startUser)
    {
        printf("Error: Grafo o usuario inicial inválido.\n");
        return;
    }
    int MAX_USERS = graph->usersNumber;
    User *stack=malloc(sizeof(User)*MAX_USERS);
    int *visited=malloc(sizeof(int)*MAX_USERS);
    for(int i=0;i<MAX_USERS;i++){
        stack[i]=NULL;
        visited[i]=0;
    }

    int top = -1;

    int startIndex = dijkstra_table_index(graph, startUser);
    if (startIndex == -1)
        return;

    visited[startIndex] = 1;
    stack[++top] = startUser;

    printf("Recorrido DFS desde %s:\n", startUser->username);

    while (top >= 0)
    {
        User currentUser = stack[top--];
        printf("- %s\n", currentUser->username);

        Edge edge = currentUser->following->next;
        while (edge)
        {
            int neighborIndex = dijkstra_table_index(graph, edge->dest);
            if (!visited[neighborIndex])
            {
                visited[neighborIndex] = 1;
                stack[++top] = edge->dest;
            }
            edge = edge->next;
        }
    }
    free(visited);
    free(stack);
}
