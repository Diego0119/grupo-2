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
5. Para comenzar a utilizar el programa, debe generar usuarios con el comando `.devgraph.out -g <cantidad de usuarios>`
> ⚠️ Asegurese que el archivo `subtopics` se encuentre en el mismo directorio del que va a ejecutarse el programa

> ⚠️ Tenga en cuenta en cuenta que el máximo número de usuarios que se pueden generar es de 100.000
 

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
| `-w, --followerlist` | Muestra tus seguidores |
| `-v, --followlist` | Muestra tus seguidos |


