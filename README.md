# Juego Pueblo Perdido

[English](#english) | [Español](#español)

<a id="english"></a>
# English

## Description

**Juego Pueblo Perdido** is an adventure game developed in C and C++ as a university project for the **Programación IV** course at the University of Deusto.

Players explore different locations, interact with characters, progress through a story by solving riddles.

The application follows a client-server architecture: the client provides the graphical interface, while the server processes requests and manages the game data.

The server handles **one client connection at a time**.

## Technologies

- **Programming languages:** C and C++
- **GUI framework:** Qt 6.9.0 (Qt Widgets)
- **Networking:** TCP sockets using Winsock2
- **Database:** SQLite 3.49.1
- **Build tools:** qmake and Make

See [REQUIREMENTS.md](REQUIREMENTS.md) for the required tools, versions and libraries.

## Architecture

The application consists of two main components:

### Client

The client provides the graphical interface and handles user interaction.

Its responsibilities include:
- Displaying the game interface and menus.
- Managing user interactions.
- Sending requests to the server.
- Processing server responses.

### Server

The server manages the game logic and data.

Its responsibilities include:
- Accepting client connections.
- Processing client requests through a custom message protocol.
- Managing users, locations, characters and riddles.
- Performing database operations.
- Sending responses to the client.

### Communication

The client and server communicate through TCP sockets using Winsock2.

The server listens on port `6000` and accepts one client connection at a time.

## Project Structure

```text
Juego-Pueblo-Perdido/
├── BD/
│   └── Database files
├── Client/
│   ├── Source files
│   └── puebloPerdido.pro
├── Server/
│   ├── Source files
│   ├── sqlite3.c
│   ├── sqlite3.h
│   └── Makefile
├── Misterio en el Pueblo Perdido.docx
├── README.md
├── REQUIREMENTS.md
└── LICENSE
```

## Execution

The client and server must be compiled and executed separately.

### 1. Compile the server

Open a terminal in the `Server` directory and run:

```bash
make
```

The Makefile uses `gcc` and `g++` from the system PATH. Make sure they point to the MinGW toolchain included with the Qt installation.

### 2. Start the server

Run the generated executable:

```bash
./Server.exe
```

The server starts listening for connections on port `6000`.

### 3. Compile the client

1. Open Qt Creator.
2. Open `Client/puebloPerdido.pro`.
3. Select the Qt 6.9.0 MinGW 64-bit kit.
4. Build the project.

The project configuration specifies the compiler executables included in the Qt installation. If Qt is installed in a different directory, update the compiler paths in the `.pro` file.

### 4. Start the client

Run the generated client executable from Qt Creator or its build directory.

The client connects to the server at `127.0.0.1:6000`.

**Important:** Start the server before launching the client.

## Academic Context

Developed as a university project for the **Programación IV** course at the University of Deusto.

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE) for details.

---

<a id="español"></a>
# Español

## Descripción

**Juego Pueblo Perdido** es un juego de aventuras desarrollado en C y C++ como proyecto universitario para la asignatura **Programación IV** de la Universidad de Deusto.

El jugador explora diferentes localizaciones, interactúa con personajes y avanza en una historia resolviendo distintos acertijos.

La aplicación sigue una arquitectura cliente-servidor: el cliente proporciona la interfaz gráfica, mientras que el servidor procesa las peticiones y gestiona los datos del juego.

El servidor admite **una única conexión de cliente simultáneamente**.

## Tecnologías

- **Lenguajes de programación:** C y C++.
- **Interfaz gráfica:** Qt 6.9.0 (Qt Widgets).
- **Redes:** Sockets TCP mediante Winsock2.
- **Base de datos:** SQLite 3.49.1.
- **Herramientas de compilación:** qmake y Make.

Consulta [REQUIREMENTS.md](REQUIREMENTS.md) para conocer las herramientas, versiones y librerías necesarias.

## Arquitectura

La aplicación está formada por dos componentes principales:

### Cliente

El cliente proporciona la interfaz gráfica y gestiona la interacción con el usuario.

Sus responsabilidades incluyen:
- Mostrar la interfaz y los menús del juego.
- Gestionar las interacciones del usuario.
- Enviar peticiones al servidor.
- Procesar las respuestas recibidas.

### Servidor

El servidor gestiona la lógica y los datos del juego.

Sus responsabilidades incluyen:
- Aceptar conexiones de clientes.
- Procesar peticiones mediante un protocolo de mensajes propio.
- Gestionar usuarios, localizaciones, personajes y acertijos.
- Realizar operaciones sobre la base de datos.
- Enviar respuestas al cliente.

### Comunicación

El cliente y el servidor se comunican mediante sockets TCP utilizando Winsock2.

El servidor escucha en el puerto `6000` y acepta una única conexión de cliente simultáneamente.

## Estructura del proyecto

```text
Juego-Pueblo-Perdido/
├── BD/
│   └── Archivos de bases de datos
├── Client/
│   ├── Archivos fuente
│   └── puebloPerdido.pro
├── Server/
│   ├── Archivos fuente
│   ├── sqlite3.c
│   ├── sqlite3.h
│   └── Makefile
├── Misterio en el Pueblo Perdido.docx
├── README.md
├── REQUIREMENTS.md
└── LICENSE
```

## Ejecución

El cliente y el servidor deben compilarse y ejecutarse por separado.

### 1. Compilar el servidor

Abre una terminal en el directorio `Server` y ejecuta:

```bash
make
```

El Makefile utiliza `gcc` y `g++` del PATH del sistema. Asegúrate de que apuntan al toolchain MinGW incluido en la instalación de Qt.

### 2. Iniciar el servidor

Ejecuta el ejecutable generado:

```bash
./Server.exe
```

El servidor comenzará a escuchar conexiones en el puerto `6000`.

### 3. Compilar el cliente

1. Abre Qt Creator.
2. Abre `Client/puebloPerdido.pro`.
3. Selecciona el kit Qt 6.9.0 MinGW de 64 bits.
4. Compila el proyecto.

La configuración del proyecto especifica las rutas de los compiladores incluidos en la instalación de Qt. Si Qt está instalado en otra ubicación, actualiza las rutas de los compiladores en el archivo `.pro`.

### 4. Iniciar el cliente

Ejecuta el ejecutable generado desde Qt Creator o desde su directorio de compilación.

El cliente se conecta al servidor en `127.0.0.1:6000`.

**Importante:** inicia el servidor antes de ejecutar el cliente.

## Contexto académico

Desarrollado como proyecto universitario para la asignatura **Programación IV** de la Universidad de Deusto.

## Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta [LICENSE](LICENSE) para más información.
