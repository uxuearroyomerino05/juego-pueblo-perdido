#include "acertijo.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace Juego;

Acertijo::Acertijo()
{
    this->pregunta = nullptr;

    this->respuesta = nullptr;
}

Acertijo::Acertijo(const char* pregunta, const char* respuesta)
{

    this->pregunta = new char[strlen(pregunta)+1];
    strcpy(this->pregunta, pregunta);

    this->respuesta = new char[strlen(respuesta)+1];
    strcpy(this->respuesta, respuesta);
}

Acertijo::~Acertijo()
{
    delete this->pregunta;
    delete this->respuesta;
}

char* Acertijo::getPregunta()
{
    return this->pregunta;
}


char* Acertijo::getRespuesta()
{
    return this->respuesta;
}

void Acertijo::setPregunta(char* pregunta)
{
    this->pregunta = new char[strlen(pregunta)+1];
    strcpy(this->pregunta, pregunta);
}

void Acertijo::setRespuesta(char* respuesta)
{
    this->respuesta = new char[strlen(respuesta)+1];
    strcpy(this->respuesta, respuesta);
}

void Acertijo::toStringAcertijo()
{
    cout<<"Pregunta: "<<this->pregunta<<", Respuesta: "<<this->respuesta<<endl;
}

char* Acertijo::convertirAChar() {
    int lenPregunta = strlen(pregunta);
    int lenRespuesta = strlen(respuesta);
    const char* separador = "#A#";
    int lenSeparador = strlen(separador);

    int totalLen = lenPregunta + lenSeparador + lenRespuesta + 1;
    char* resultado = new char[totalLen];

    strcpy(resultado, pregunta);
    strcat(resultado, separador);
    strcat(resultado, respuesta);

    return resultado;
}

Acertijo::Acertijo(const char* linea) {
    const char* separador = "#A#";
    const char* pos = strstr(linea, separador);

    if (pos != nullptr) {
        int lenPregunta = pos - linea;
        pregunta = new char[lenPregunta + 1];
        strncpy(pregunta, linea, lenPregunta);
        pregunta[lenPregunta] = '\0';

        const char* inicioRespuesta = pos + strlen(separador);
        respuesta = new char[strlen(inicioRespuesta) + 1];
        strcpy(respuesta, inicioRespuesta);
    } else {
        // Si no hay separador, todo es pregunta
        pregunta = strdup(linea);
        respuesta = new char[1];
        respuesta[0] = '\0';
    }
}

