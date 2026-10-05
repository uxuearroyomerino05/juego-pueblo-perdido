#include "protocoloMensages.h"

#include <winsock2.h>
#include <iostream>
#include <string>
#include <cstring>
#include "ficheros.h"


void enviarYRecibir(SOCKET s, const char* mensaje, char** respuesta) {

    const int TAM_BUFFER = 65536;
    char buffer[TAM_BUFFER], logMsg[TAM_BUFFER];

    if (*respuesta != nullptr) {
        delete[] *respuesta;
        *respuesta = nullptr;
    }

    // Enviar mensaje
    sprintf(logMsg, "Enviando: %s", mensaje);
	escribirFicheroLog(logMsg);
    if (send(s, mensaje, strlen(mensaje) + 1, 0) == SOCKET_ERROR) {
        sprintf(logMsg, "Error al enviar. Código: : %d", WSAGetLastError());
	    escribirFicheroLog(logMsg);
        return;
    }

    // Recibir respuesta (una sola vez)
    memset(buffer, 0, TAM_BUFFER);
    int bytesRecibidos = recv(s, buffer, TAM_BUFFER - 1, 0);
    if (bytesRecibidos == SOCKET_ERROR) {
        sprintf(logMsg, "Error al enviar. Código: : %d", WSAGetLastError());
	    escribirFicheroLog(logMsg);
        return;
    }

    if (bytesRecibidos > 0) {
        buffer[bytesRecibidos] = '\0';  // Null-terminate the buffer

        // Verificar que la respuesta no esté vacía
        if (bytesRecibidos > 0) {
            sprintf(logMsg, "Recibido: %s", buffer);
	        escribirFicheroLog(logMsg);

            // Separar por el primer '|'
            char* separador = strchr(buffer, '|');
            if (separador != nullptr) {
                *separador = '\0';  // Terminamos el comando
                const char* comando = buffer;
                const char* contenido = separador + 1;

                if (strcmp(comando, "ERR") == 0) {
                    sprintf(logMsg, "Error del servidor: %s", contenido);
	                escribirFicheroLog(logMsg);
                    *respuesta = nullptr; // No devolver nada si hay error
                } else {
                    // Asignar memoria solo si no es error
                    *respuesta = new char[strlen(contenido) + 1];
                    strcpy(*respuesta, contenido);
	                escribirFicheroLog("Respuesta devuelta correctamente");
                }

            } else {
	            escribirFicheroLog("Error: Respuesta malformada (sin '|')");
                *respuesta = nullptr;
            }

        } else {
            escribirFicheroLog("Error: La respuesta está vacía.");
            *respuesta = nullptr;
        }

    } else if (bytesRecibidos == 0) {
        escribirFicheroLog("<< Conexión cerrada por el servidor.");
        *respuesta = nullptr;
    } else {
        sprintf(logMsg, "Error al recibir. Código: %d", WSAGetLastError());
	    escribirFicheroLog(logMsg);
        *respuesta = nullptr;
    }
}

