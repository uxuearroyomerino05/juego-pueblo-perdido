#include <winsock2.h>
#include <string>

#ifndef _PROTOCOLO_MENSAGES_H_ 
#define _PROTOCOLO_MENSAGES_H_

void enviarYRecibir(SOCKET s, const char* mensaje, char** respuesta);

#endif