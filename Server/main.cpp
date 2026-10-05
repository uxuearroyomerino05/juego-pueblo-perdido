#include "jugador.h"
#include "acertijo.h"
#include "personaje.h"
#include "zona.h"
#include "pueblo.h"

#include "ConversorDatos.h"

extern "C" {
    #include "jugador_Struct.h"
    #include "acertijo_Struct.h"
    #include "personaje_Struct.h"
    #include "zona_Struct.h"
    #include "pueblo_Struct.h"
	#include "sqlite3.h"
	#include "baseDeDatos.h"
	#include "ficheros.h"
}

using namespace Juego;

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

int BufferSize = 65536;

int main(int argc, char *argv[]) {

    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[BufferSize], recvBuff[BufferSize];
    char logMsg[BufferSize];

    escribirFicheroLog("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        sprintf(logMsg, "Error al iniciar Winsock: %d\n", WSAGetLastError());
        escribirFicheroLog(logMsg);
        return -1;
    }

    escribirFicheroLog("Winsock inicializado.\n");

    // Crear socket
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        sprintf(logMsg, "No se pudo crear el socket: %d\n", WSAGetLastError());
        escribirFicheroLog(logMsg);
        WSACleanup();
        return -1;
    }

    escribirFicheroLog("Socket creado.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        sprintf(logMsg, "Falló bind: %d\n", WSAGetLastError());
        escribirFicheroLog(logMsg);
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    escribirFicheroLog("Bind realizado correctamente.\n");

    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        sprintf(logMsg, "Falló listen: %d\n", WSAGetLastError());
        escribirFicheroLog(logMsg);
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    escribirFicheroLog("Esperando conexiones entrantes...\n");

    int stsize = sizeof(struct sockaddr);
    comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
    if (comm_socket == INVALID_SOCKET) {
        sprintf(logMsg, "Falló accept: %d\n", WSAGetLastError());
        escribirFicheroLog(logMsg);
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    sprintf(logMsg, "Conexión entrante desde: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    escribirFicheroLog(logMsg);

    closesocket(conn_socket);
    escribirFicheroLog("Esperando mensajes del cliente...\n");

    do {
        escribirFicheroLog("Recibiendo mensaje...\n");
        memset(recvBuff, 0, BufferSize);
        int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        sprintf(logMsg, "Bytes recibidos: %d\n", bytes);
        escribirFicheroLog(logMsg);

		if (bytes > 0) {
			sprintf(logMsg, "Data received: %s \n", recvBuff);
			escribirFicheroLog(logMsg);

			escribirFicheroLog("Sending reply... \n");

            if (strcmp(recvBuff, "Bye") == 0){
				strcpy(sendBuff, "OK|Cerrando Servidor \n");
				send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
				escribirFicheroLog("OK|Cerrando Servidor \n");
				break;
			}
			
			char* comando = strtok(recvBuff, "|");
			if (comando == nullptr) {
				strcpy(sendBuff, "ERR|Formato de comando incorrecto");
				send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
				escribirFicheroLog("ERR|Formato de comando incorrecto");
				break;
			}

			char* parametros[10];
			int numParametros = 0;

			char* token = strtok(nullptr, "|");
			while (token != nullptr && numParametros < 10) {
				parametros[numParametros++] = token;
				token = strtok(nullptr, "|");
			}

			// === INICIO DE LOS COMANDOS ===

			if (strcmp(comando, "INSERT_JUGADOR") == 0) {
				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
			
					const char* datosJugador = parametros[0];
				
					try {
						// 1. Convertir texto a clase Jugador
						Usuario::Jugador* jugadorClase = new Usuario::Jugador(datosJugador);
				
						// 2. Convertir clase Jugador a estructura C
						Jugador_Struct* jugadorStruct = convertirAJugadorStruct(jugadorClase);
				
						// 3. Insertar en base de datos
						insertarJugador(*jugadorStruct);
				
						// 4. Enviar confirmación
						strcpy(sendBuff, "OK|Jugador insertado correctamente");
				
						// Liberar memoria
						delete jugadorClase;
						delete jugadorStruct;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al procesar el jugador");
					}
				}

			} else if (strcmp(comando, "INSERT_PUEBLO") == 0) {
				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoPueblo = parametros[0];
			
					try {
						Pueblo* pueblo = new Pueblo(textoPueblo);
						Pueblo_Struct* puebloStruct = convertirAPuebloStruct(pueblo);
			
						insertarPueblo(*puebloStruct);
			
						strcpy(sendBuff, "OK|Pueblo insertado correctamente");
			
						delete pueblo;
						delete puebloStruct;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al insertar el pueblo");
					}
				}
				
			} else if (strcmp(comando, "INSERT_ZONA") == 0) {
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoZona = parametros[0];
					const char* nombrePueblo = parametros[1];
			
					try {
						Zona* zona = new Zona(textoZona);
						Zona_Struct* zonaStruct = convertirAZonaStruct(zona);
						insertarZona(*zonaStruct, (char*)nombrePueblo);
			
						strcpy(sendBuff, "OK|Zona insertada correctamente");
			
						delete zona;
						delete zonaStruct;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al insertar la zona");
					}
				}
			} else if (strcmp(comando, "INSERT_PERSONAJE") == 0) {
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoPersonaje = parametros[0];
					const char* nombreZona = parametros[1];
			
					try {
						// 1. Convertir texto a clase Personaje
						Juego::Personaje* personajeClase = new Juego::Personaje(textoPersonaje);
			
						// 2. Convertir personaje a estructura
						Personaje_Struct* personajeStruct = convertirAPersonajeStruct(personajeClase);
			
						// 3. Insertar personaje completo
						insertarPersonaje(*personajeStruct, nombreZona);
			
						// 4. Enviar confirmación
						strcpy(sendBuff, "OK|Personaje insertado correctamente");
			
						// 5. Liberar memoria
						delete personajeClase;
						delete personajeStruct;
			
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al insertar el personaje");
					}
				}
			} else if (strcmp(comando, "GET_JUGADOR") == 0) {
				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* nombreJugador = parametros[0];
			
					try {
						// 1. Obtener la estructura Jugador desde la base de datos
						Jugador_Struct jugadorStruct = obetenerJugadorPorNombre((char*)nombreJugador);
						// 2. Convertir estructura C a clase C++
						Usuario::Jugador* jugadorClase = convertirAJugadorClase(&jugadorStruct);
						// 3. Convertir clase a texto
						char* jugadorTexto = jugadorClase->convertirAChar();
						// 4. Enviar mensaje al cliente
						strcpy(sendBuff, "DATA|");
						strcat(sendBuff, jugadorTexto);
			
						// Liberar memoria
						delete jugadorClase;
						delete[] jugadorTexto;
					} catch (...) {
						strcpy(sendBuff, "ERR|Jugador no encontrado o error al procesar");
					}
				}
			} else if (strcmp(comando, "GET_PUEBLO") == 0) {
				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* nombrePueblo = parametros[0];
			
					try {
						// 1. Obtener la estructura del pueblo desde la base de datos
						Pueblo_Struct puebloStruct = obtenerPueblo((char*)nombrePueblo);
						// 2. Convertir la estructura a clase C++ (todo lo interno ya se convierte)
						Juego::Pueblo* puebloClase = convertirAPuebloClase(&puebloStruct);
						// 3. Convertir la clase a texto
						char* textoPueblo = puebloClase->convertirAChar();
						// 4. Enviar como respuesta
						strcpy(sendBuff, "DATA|");
						strcat(sendBuff, textoPueblo);
			
						// 5. Liberar memoria
						delete puebloClase;
						delete[] textoPueblo;
			
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al obtener el pueblo");
					}
				}
			} else if (strcmp(comando, "GET_NOMBRES_PUEBLO") == 0) {
				if (numParametros != 0) {
					strcpy(sendBuff, "ERR|Este comando no recibe parámetros");
				} else {
					try {
						int cantidad = 0;
						char** nombres = obtenerNombresPueblo(&cantidad);
									
						if (cantidad == 0 || nombres == nullptr) {
							strcpy(sendBuff, "DATA|");
						} else {
							// Construimos la cadena uniendo con %L%
							strcpy(sendBuff, "DATA|");
							for (int i = 0; i < cantidad; ++i) {
								strcat(sendBuff, nombres[i]);
								if (i < cantidad - 1)
									strcat(sendBuff, "%L%");
							}
						}
			
						// Liberar memoria si es dinámica
						for (int i = 0; i < cantidad; ++i) {
							delete[] nombres[i];
						}
						delete[] nombres;
			
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al obtener nombres de pueblos");
					}
				}
			} else if (strcmp(comando, "MOD_PUEBLO") == 0) {
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoModificado = parametros[0];
					const char* textoOriginal = parametros[1];
			
					try {
						// 1. Convertir texto a clases Pueblo
						Juego::Pueblo* puebloNuevo = new Juego::Pueblo(textoModificado);
						Juego::Pueblo* puebloViejo = new Juego::Pueblo(textoOriginal);
			
						// 2. Convertir clases a estructuras
						Pueblo_Struct* structNuevo = convertirAPuebloStruct(puebloNuevo);
						Pueblo_Struct* structViejo = convertirAPuebloStruct(puebloViejo);
			
						// 3. Llamar a la base de datos
						modificarPueblo(*structNuevo, *structViejo);
			
						// 4. Responder al cliente
						strcpy(sendBuff, "OK|Pueblo modificado correctamente");
			
						// 5. Liberar memoria
						delete puebloNuevo;
						delete puebloViejo;
						delete structNuevo;
						delete structViejo;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al modificar el pueblo");
					}
				}
			} else if (strcmp(comando, "MOD_ZONA") == 0) {
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoNueva = parametros[0];
					const char* textoAnterior = parametros[1];
			
					try {
						Juego::Zona* nueva = new Juego::Zona(textoNueva);
						Juego::Zona* anterior = new Juego::Zona(textoAnterior);
			
						Zona_Struct* structNueva = convertirAZonaStruct(nueva);
						Zona_Struct* structAnterior = convertirAZonaStruct(anterior);
						modificarZona(*structNueva, *structAnterior);
			
						strcpy(sendBuff, "OK|Zona modificada correctamente");
			
						delete nueva;
						delete anterior;
						delete structNueva;
						delete structAnterior;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al modificar la zona");
					}
				}
			} else if (strcmp(comando, "MOD_PERSONAJE") == 0) {
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoNuevo = parametros[0];
					const char* textoAnterior = parametros[1];
			
					try {
						Personaje* nuevo = new Personaje(textoNuevo);
						Personaje* anterior = new Personaje(textoAnterior);
			
						Personaje_Struct* structNuevo = convertirAPersonajeStruct(nuevo);
						Personaje_Struct* structAnterior = convertirAPersonajeStruct(anterior);
			
						modificarPersonaje(*structNuevo, *structAnterior);
			
						strcpy(sendBuff, "OK|Personaje modificado correctamente");
			
						delete nuevo;
						delete anterior;
						delete structNuevo;
						delete structAnterior;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al modificar el personaje");
					}
				}
			} else if (strcmp(comando, "DEL_PUEBLO") == 0) {

				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoPueblo = parametros[0];
			
					try {
						Pueblo* pueblo = new Pueblo(textoPueblo);
						Pueblo_Struct* puebloStruct = convertirAPuebloStruct(pueblo);
			
						eliminarPueblo(*puebloStruct);
			
						strcpy(sendBuff, "OK|Pueblo eliminada correctamente");
			
						delete pueblo;
						delete puebloStruct;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al eliminar el pueblo");
					}
				}

			}  else if (strcmp(comando, "DEL_ZONA") == 0) {

				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoZona = parametros[0];
			
					try {
						Zona* zona = new Zona(textoZona);
						Zona_Struct* zonaStruct = convertirAZonaStruct(zona);
			
						eliminarZona(*zonaStruct);
			
						strcpy(sendBuff, "OK|Zona eliminada correctamente");
			
						delete zona;
						delete zonaStruct;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al eliminar la zona");
					}
				}

			} else if (strcmp(comando, "DEL_PERSONAJE") == 0) {
				if (numParametros != 1) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoPersonaje = parametros[0];
			
					try {
						Personaje* personaje = new Personaje(textoPersonaje);
						Personaje_Struct* personajeStruct = convertirAPersonajeStruct(personaje);
			
						eliminarPersonaje(*personajeStruct);
			
						strcpy(sendBuff, "OK|Personaje eliminado correctamente");
			
						delete personaje;
						delete personajeStruct;
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al eliminar el personaje");
					}
				}
			} else if (strcmp(comando, "ANADIR_OBJETOS_JUGADOR") == 0) {
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					const char* textoJugador = parametros[0];
					const char* nombrePueblo = parametros[1];
			
					try {
						// 1. Convertir el texto a clase Jugador
						Usuario::Jugador* jugadorClase = new Usuario::Jugador(textoJugador);
			
						// 2. Convertir a estructura C
						Jugador_Struct* jugadorStruct = convertirAJugadorStruct(jugadorClase);
			
						// 3. Añadir los objetos directamente
						obtenerObjetosPorIdJugadorPorIdPueblo(jugadorStruct, (char*)nombrePueblo);

						// 4. Convertir de nuevo la estructura a clase actualizada
						delete jugadorClase; // destruimos la versión anterior
						jugadorClase = convertirAJugadorClase(jugadorStruct);
						jugadorClase->toStringJugador();
						// 5. Convertir a texto actualizado
						char* jugadorTexto = jugadorClase->convertirAChar();
			
						// 6. Enviar
						strcpy(sendBuff, "DATA|");
						strcat(sendBuff, jugadorTexto);
			
						// 7. Liberar memoria
						delete jugadorClase;
						delete jugadorStruct;
						delete[] jugadorTexto;
			
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al añadir los objetos al jugador");
					}
				}
			} else if (strcmp(comando, "MOD_CONTRASENA") == 0){
				if (numParametros != 2) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					char* nombreJugador = parametros[0];
					char* contrasena = parametros[1];
			
					try {
								
						// 1. Modificar la contraseña
						modificarContrasena(nombreJugador, contrasena);
			
						// 2. Enviar
						strcpy(sendBuff, "OK|Contraseña modificada");
			
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al modificar la contraseña");
					}
				}

			} else if (strcmp(comando, "INSERTAR_OBJETOS_JUGADOR") == 0) {
				if (numParametros != 3) {
					strcpy(sendBuff, "ERR|Número incorrecto de parámetros");
				} else {
					char* nombreJugador = parametros[0];
					char* nombrePueblo = parametros[1];
					char* objeto = parametros[2];
			
					try {			
						// 1. Insertar los objetos directamente
						insertarObjetoJugador(nombreJugador, nombrePueblo, objeto);
			
						// 2. Enviar
						strcpy(sendBuff, "OK|Insertado objeto jugador correctamente");
			
					} catch (...) {
						strcpy(sendBuff, "ERR|Error al añadir los objetos al jugador");
					}
				}
			} else {
				strcpy(sendBuff, "ERR|Comando incorrecto");
			}
			
            // === FIN DE LOS COMANDOS ===
			escribirFicheroLog("Paso final: Enviando mensage");
			send(comm_socket, sendBuff, strlen(sendBuff)+1, 0);
			sprintf(logMsg, "Data sent: %s \n", sendBuff);
			escribirFicheroLog(logMsg);

		}else if (bytes <= 0) {
			escribirFicheroLog("El cliente cerró la conexión.\n");
			break;
			
		} else {
			escribirFicheroLog("Error en recv\n");
			break;
		}

	} while (1);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();
	escribirFicheroLog("Servidor cerrado correctamente.\n");

	return 0;
}
