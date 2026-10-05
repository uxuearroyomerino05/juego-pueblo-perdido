#include <QApplication>
#include "menuInicio.h"

#include "jugador.h"
#include "pueblo.h"
#include "zona.h"
#include "personaje.h"
#include "acertijo.h"

using namespace Juego;
using namespace Usuario;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#include <winsock2.h>
#include <iostream>
#include <string>
#include <cstring>
#include "protocoloMensages.h"
#include "ficheros.h"

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char logMsg[16384];

    escribirFicheroLog("Inicializando Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        sprintf(logMsg, "Error:  %d", WSAGetLastError());
	    escribirFicheroLog(logMsg);
        return -1;
    }

    escribirFicheroLog("Winsock inicializado.");

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        sprintf(logMsg, "Error al crear socket:  %d", WSAGetLastError());
	    escribirFicheroLog(logMsg);
        WSACleanup();
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        sprintf(logMsg, "Error de conexión:  %d", WSAGetLastError());
	    escribirFicheroLog(logMsg);
        closesocket(s);
        WSACleanup();
        return -1;
    }

    sprintf(logMsg, "Conectado al servidor en %s : %i ", SERVER_IP, SERVER_PORT);
	escribirFicheroLog(logMsg);

    QApplication app(argc, argv);
    MenuInicio* inicio = new MenuInicio(s);
    inicio->show();
    return app.exec();

}

