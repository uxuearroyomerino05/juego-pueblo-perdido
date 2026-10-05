# Requirements — Juego Pueblo Perdido

## 1. Operating System

The project uses Windows-specific networking APIs through Winsock.

- **Platform:** Windows
- **Architecture:** The client configuration included in the project targets 64-bit MinGW.

## 2. Programming Languages

- C
- C++

The server contains both C and C++ source files.

## 3. Qt Development Environment

The client uses Qt Widgets for its graphical interface.

The included project configuration indicates the following environment:

- **Qt version:** 6.9.0
- **Build system:** qmake
- **Compiler toolchain:** MinGW 64-bit
- **Compiler toolchain directory:** `C:/Qt/Tools/mingw1310_64/`
- **GCC/G++ version indicated by the toolchain path:** 13.1.0

The client project file is located at `Client/puebloPerdido.pro`.

The Qt modules used by the client are:

- Qt Core
- Qt GUI
- Qt Widgets

A compatible Qt installation must include these modules and the matching MinGW toolchain for compiling C and C++.

## 4. Networking

The client and server communicate through TCP sockets using the Windows Winsock API.

- **Networking API:** Winsock2
- **Default server port:** 6000

Ensure that the selected port is available and that local firewall settings allow the connection.

## 5. Database

The server uses SQLite for persistent data storage.

- **Database engine:** SQLite
- **Source files included:** `Server/sqlite3.c` and `Server/sqlite3.h`
- **Database files:** Located in `BD/`

The project includes SQLite source code, so a separate SQLite installation may not be necessary for the database component.

## 6. Build Requirements

### Client

The client requires:
- Qt 6.9.0, or a compatible installation
- Qt Widgets
- qmake
- A compatible 64-bit MinGW toolchain
- Windows SDK/system libraries required by the compiler

The `.pro` file currently contains absolute compiler paths. These paths may need to be updated to match the local Qt installation.

### Server

The server uses C and C++ source files, Winsock, and SQLite.

**The exact compiler version and server build procedure still need to be verified** against the original development environment before this project can be considered reproducible from a clean installation.

## 7. Configuration Notes

- The client and server are separate applications.
- Start the server before launching the client.
- Check the configured IP address and port if the client cannot connect.
- The server is designed to handle one client connection at a time.

## 8. Verified Environment

The versions listed above are based on the project configuration and build artifacts included in the repository. They should be checked against the original development environment before being treated as a complete record of the versions used during development.
