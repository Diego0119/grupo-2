#include "heaps.h"

/**
 * @brief funcion para insertar una nueva publicación en el heap
 *
 * @param h cola de prioridad
 * @param user_name nombre del usuario
 * @param priority prioridad de la publicacion
 * @param content contenido de la publicacion
 */
void insert_new_post(heap* h, const char* user_name, int priority, const char* content)
{
    if (h->size >= MAX_HEAP_SIZE) {
        printf("El heap está lleno.\n");
        return;
    }

    post new_post;
    strcpy(new_post.user_name, user_name);
    new_post.priority = priority;
    strcpy(new_post.content, content);

    // Insertar al final y luego hacer heapify_up
    h->posts[h->size] = new_post;
    h->size++;
    heapify_up(h, h->size - 1);
}

/**
 * @brief funcion para asegurar que el heap mantenga la propiedad (heapify_up)
 *
 * @param h cola de prioridad
 * @param user_name nombre del usuario
 */
void heapify_up(heap* h, int index)
{
    while (index > 0 && h->posts[index].priority > h->posts[(index - 1) / 2].priority) {
        // Intercambiar
        post temp = h->posts[index];
        h->posts[index] = h->posts[(index - 1) / 2];
        h->posts[(index - 1) / 2] = temp;

        index = (index - 1) / 2;
    }
}

/**
 * @brief funcion para extraer el máximo (el nodo más prioritario) y lo muestra (Se utiliza en la función ver_posts)
 *
 * @param h cola de prioridad
 */
void extract_max(heap* h)
{
    if (h->size == 0) {
        printf("\nNo hay mas publicaciones, intente mas tarde\n");
        return;
    }

    printf("%s\n\n|----------\n  %s\n|----------\n", h->posts[0].user_name, h->posts[0].content);

    // Mover el último nodo a la raíz y hacer heapify_down
    h->posts[0] = h->posts[h->size - 1];
    h->size--;
    // Restaurar la propiedad del heap
    heapify_down(h, 0);
}

/**
 * @brief funcion para hacer heapify_down (restaurar la propiedad después de extraer el máximo)
 *
 * @param h cola de prioridad
 * @param index indice del nodo
 */
void heapify_down(heap* h, int index)
{
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
        // Intercambiar
        post temp = h->posts[index];
        h->posts[index] = h->posts[largest];
        h->posts[largest] = temp;

        heapify_down(h, largest);
    }
}

/**
 * @brief funcion para imprimir el heap
 *
 * @param h cola de prioridad
 */
void print_heap(heap* h)
{
    if (h->size == 0) {
        printf("El heap está vacío.\n");
        return;
    }

    printf("Contenido del heap: %d\n", h->size);
    for (int i = 0; i < h->size; i++) {
        printf("Usuario: %s, Prioridad: %d, Contenido: %s\n",
            h->posts[i].user_name, h->posts[i].priority, h->posts[i].content);
    }
}

/**
 * @brief funcion para visualizar publicaciones del heap en consola y orden de prioridad
 *
 * @param h cola de prioridad
 */
void ver_posts(heap* h)
{
    int opcion = 1;

    while (opcion != 2) {
        printf("\n\t\tPUBLICACIONES\n");

        // Verifica si hay publicaciones y extrae la más relevante (máximo)
        switch (opcion) {
            case 1:
                extract_max(h);
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
