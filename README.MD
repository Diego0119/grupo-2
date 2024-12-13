<h1 align="center">DevGraph 🌐💬</h1>
<h3 align="center">Simulación de Social Network</h3>

<p align="center">
  <img src="https://img.shields.io/badge/C-00599C?logo=c&logoColor=white">
  <img src="https://img.shields.io/badge/version-1.0-blue?label=version
  ">
</p>

***

Este repositorio contiene el desarrollo de una **Social Network**, para el curso de **Estructuras de Datos** 2024/2



### 👨‍💻 Desarrollado por:
[Duvan Figueroa](https://github.com/HisokaMorow1), [Iván Mansilla](https://github.com/ivnmansi), [Franco Aguilar](https://github.com/faguilardc), [Miguel Maripillan](https://github.com/mihel1) y [Diego Sanhueza](https://github.com/Diego0119)

## Ejecución del programa
Para ejecutar el programa, siga los siguientes pasos:
1. Clonar el repositorio
2. Generar las carpetas necesarias con el comando `make folders`
3. Compilar el programa con el comando `make`
4. Ejecutar el programa con `make run` o entrando a la carpeta `build` y ejecutando el archivo `devgraph.out`
> ⚠️ Asegurese que el archivo `subtopics` se encuentre en el mismo directorio del que va a ejecutarse el programa

## Comandos disponibles
| Comando | Descripción |
| --- | --- |
| `-h, --help` | Muestra la ayuda del programa |
| `-g, --generate <cantidad>` | Genera usuarios aleatorios |
| `-c, --clear` | Borra la base de datos |
| `-a, --all` | Lista todos los usuarios registrados |
| `-t, --topics` | Muestra todos los tópicos disponibles en DevGraph |
| `-l, --login` | Inicia sesión |
| `-o, --logout` | Cerrar sesión |
| `-r, --register` | Registra un nuevo usuario |
| `-d, --delete` | Elimina la cuenta de la sesión iniciada |
| `-e, --edit` | Permite editar la información de la sesión iniciada |
| `-p, --post` | Publica una publicación |
| `-m, --me` | Muestra el perfil del usuario actual |
| `-u, --user <usuario>` | Muestra el perfil de un usuario |
| `-f, --follow <usuario>` | Sigue a un usuario |
| `-n, --unfollow <usuario>` | Deja de seguir a un usuario |
| `-q, --feed` | Muestra los posts seleccionados |
| `-x, --connect` | Muestra los usuarios recomendados |







## Lista de tareas
- [X] Implementar tablas hash (Duván)
- [ ] Funciones de usuarios basándose en afinidades o popularidad
- [X] Organizar publicaciones (Duván)
- [X] Estructura de grafos (Diego y Miguel)
- [X] Algoritmos BFS/DFS (Miguel)
- [X] Implementar Dijkstra para encontrar usuarios cercanos (Diego)
- [X] Implementar las listas enlazadas para manejar las publicaciones (Iván)
- [X] Crear estructuras de usuarios (contempla la lista enlazada de publicaciones) (Iván)
- [X] Agregar temas/tópicos en la estructura de usuarios y publicaciones (Iván)
- [X] Implementar Jaccard en usuarios y publicaciones (Iván)
- [X] Asignación de intereses a usuarios (Iván)
- [X] Implementar colas de prioridad (Franco)
- [ ] Algoritmo de recomendación de publicaciones
- [ ] Algoritmo de recomendación de usuarios
- [X] Guardar información de los usuarios (nube) (Miguel)
- [X] Cargar usuarios por medio de un txt de entrada (Miguel)
- [X] Iniciar sesión y publicar cosas según el usuario iniciado (Iván)
- [X] Comandos para publicar, borrar, ver usuarios, perfil, publicaciones, etc (Iván)
- [ ] Ordenar main para el programa final 

### Urgente
- [ ] Arreglar carga y guardado de publicaciones
- [ ] Arreglar carga de seguidores y seguidos

### Extras
- [X] Perfil psicológico (Diego)
- [X] Usuarios aleatorios (Diego)

