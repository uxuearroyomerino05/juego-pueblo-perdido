# Requirements — Juego Pueblo Perdido

[English](#english) | [Español](#español)

<a id="english"></a>
# English

## Operating System

- Windows (required for the Winsock networking implementation).

## Development Environment

| Tool | Version / Configuration |
|---|---|
| Qt | 6.9.0 |
| Qt Creator | Compatible with Qt 6.9.0 |
| MinGW | 13.1.0, 64-bit |
| qmake | 3.1 |
| C++ standard (server) | C++11 |
| Make | GNU Make compatible with the included Makefile |

## Libraries and Modules

| Library / Module | Purpose |
|---|---|
| Qt Core | Core application functionality |
| Qt GUI | GUI integration |
| Qt Widgets | Graphical user interface |
| Winsock2 | TCP socket communication |
| SQLite 3.49.1 | Database management |

SQLite source files (`sqlite3.c` and `sqlite3.h`) are included in the repository.

Winsock2 is provided by the Windows development environment and linked through `ws2_32`.

## Compiler Configuration

The client project uses the MinGW toolchain included with Qt 6.9.0.

The `.pro` file specifies the compiler executable paths:

- `C:/Qt/Tools/mingw1310_64/bin/gcc.exe`
- `C:/Qt/Tools/mingw1310_64/bin/g++.exe`

The server Makefile uses `gcc` and `g++` from the system PATH.

**Important:** If another MinGW/GCC installation is available, make sure the server uses the same MinGW toolchain included with Qt. This avoids mixing incompatible compiler versions.

If Qt is installed in a different directory, update the compiler paths in `Client/puebloPerdido.pro`.

## Network Configuration

- Protocol: TCP
- Networking API: Winsock2
- Default server address: `127.0.0.1`
- Default server port: `6000`
- Maximum simultaneous client connections: 1

---

<a id="español"></a>
# Español

## Sistema operativo

- Windows (necesario para la implementación de red mediante Winsock).

## Entorno de desarrollo

| Herramienta | Versión / Configuración |
|---|---|
| Qt | 6.9.0 |
| Qt Creator | Compatible con Qt 6.9.0 |
| MinGW | 13.1.0, 64 bits |
| qmake | 3.1 |
| Estándar de C++ (servidor) | C++11 |
| Make | GNU Make compatible con el Makefile incluido |

## Librerías y módulos

| Librería / Módulo | Uso |
|---|---|
| Qt Core | Funcionalidad principal de la aplicación |
| Qt GUI | Integración gráfica |
| Qt Widgets | Interfaz gráfica |
| Winsock2 | Comunicación mediante sockets TCP |
| SQLite 3.49.1 | Gestión de la base de datos |

Los archivos fuente de SQLite (`sqlite3.c` y `sqlite3.h`) están incluidos en el repositorio.

Winsock2 está proporcionado por el entorno de desarrollo de Windows y se enlaza mediante `ws2_32`.

## Configuración del compilador

El proyecto del cliente utiliza el toolchain MinGW incluido en Qt 6.9.0.

El archivo `.pro` especifica las rutas de los ejecutables del compilador:

- `C:/Qt/Tools/mingw1310_64/bin/gcc.exe`
- `C:/Qt/Tools/mingw1310_64/bin/g++.exe`

El Makefile del servidor utiliza `gcc` y `g++` disponibles en el PATH del sistema.

**Importante:** Si hay otra instalación de MinGW/GCC disponible, asegúrate de que el servidor utiliza el mismo toolchain MinGW incluido en Qt. Así se evita mezclar versiones incompatibles del compilador.

Si Qt está instalado en otra ubicación, actualiza las rutas de los compiladores en `Client/puebloPerdido.pro`.

## Configuración de red

- Protocolo: TCP.
- API de red: Winsock2.
- Dirección del servidor por defecto: `127.0.0.1`.
- Puerto del servidor por defecto: `6000`.
- Máximo de clientes simultáneos: 1.
