# Juego Pueblo Perdido

A client-server adventure game developed in C and C++ as a university project for the **Programación IV** course at the University of Deusto.

The game allows players to interact with a virtual world, explore locations, interact with characters, and solve mysteries through a graphical user interface.

## Project Overview

The application follows a client-server architecture. The client provides the graphical interface and communicates with the server through TCP sockets. The server processes client requests and manages persistent game data using SQLite.

The server is designed to handle **one client connection at a time**.

## Main Features

- Graphical user interface built with Qt.
- Client-server communication using TCP sockets.
- Server-side request processing.
- Persistent data storage using SQLite.
- Player registration and login.
- Interaction with characters, locations, and game content.
- Mystery-solving gameplay.
- Logging and message handling.

## Architecture

The project is divided into two main components:

### Client

The client is responsible for the graphical interface, user interaction, and communication with the server.

Main technologies:
- C++
- Qt Widgets
- TCP sockets using Winsock

### Server

The server processes client requests and manages the game's data.

Main technologies:
- C and C++
- TCP sockets using Winsock
- SQLite
- Data conversion between C structures and C++ classes

## Technologies

- **Languages:** C and C++
- **GUI framework:** Qt Widgets
- **Networking:** TCP sockets / Winsock
- **Database:** SQLite
- **Build system (client):** qmake

For detailed dependencies and environment requirements, see [REQUIREMENTS.md](REQUIREMENTS.md).

## Project Structure

- `Client/` — Graphical client application.
- `Server/` — Server logic, networking, and database operations.
- `BD/` — SQLite database files.

## Running the Project

The client and server must be compiled and executed separately.

1. Compile and start the server.
2. Compile and start the client.
3. Connect to the server using the configured address and port.

See [REQUIREMENTS.md](REQUIREMENTS.md) for the required development environment and build notes. The server and client are compile for testing them, the compile files are in the Server and Client/build/release folders.

## Academic Context

This project was developed as part of the **Programación IV** course at the University of Deusto in May/June 2025.

It was designed as an academic exercise in C/C++, networking, client-server communication, graphical interfaces, and database integration.

## License

This project is distributed under the MIT License. See [LICENSE](LICENSE) for details.