#include "personaje.h"
#include "acertijo.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace Juego;

Personaje::Personaje(const char* nombre, const char* dialogo,const  char* pista, Acertijo* acertijo, const char* objeto)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);

    this->dialogo = new char[strlen(dialogo)+1];
    strcpy(this->dialogo, dialogo);

    this->pista = new char[strlen(pista)+1];
    strcpy(this->pista, pista);

    this->acertijo = acertijo;

    this->objeto = objeto ? new char[strlen(objeto)+1] : nullptr;
    if (this->objeto) strcpy(this->objeto, objeto);

}

Personaje::Personaje(const char* nombre, const char* dialogo,const  char* pista, Acertijo* acertijo)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);

    this->dialogo = new char[strlen(dialogo)+1];
    strcpy(this->dialogo, dialogo);

    this->pista = new char[strlen(pista)+1];
    strcpy(this->pista, pista);

    this->acertijo = acertijo;

    this->objeto = nullptr;

}

Personaje::Personaje(Personaje* p)
{
    this->nombre = new char[strlen(p->nombre)+1];
    strcpy(this->nombre, p->nombre);

    this->dialogo = new char[strlen(p->dialogo)+1];
    strcpy(this->dialogo, p->dialogo);

    this->pista = new char[strlen(p->pista)+1];
    strcpy(this->pista, p->pista);

    this->acertijo = new Acertijo(p->acertijo);

    this->objeto = p->objeto ? new char[strlen(p->objeto)+1] : nullptr;
    if (this->objeto) strcpy(this->objeto, p->objeto);
}

Personaje::Personaje()
{
    this->nombre = nullptr;

    this->dialogo = nullptr;

    this->pista = nullptr;

    this->acertijo = nullptr;

    this->objeto = nullptr;
}

Personaje::~Personaje()
{
    delete this->nombre;
    delete this->dialogo;
    delete this->pista;
    delete this->objeto;
}

char* Personaje::getNombre()
{
    return this->nombre;
}

char* Personaje::getDialogo()
{
    return this->dialogo;
}

char* Personaje::getPista()
{
    return this->pista;
}

Acertijo* Personaje::getAcertijo()
{
    return this->acertijo;
}

char* Personaje::getObjeto()
{
    return this->objeto;
}

void Personaje::setNombre(char* nombre)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);
}

void Personaje::setDialogo(char* dialogo)
{
    this->dialogo = new char[strlen(dialogo)+1];
    strcpy(this->dialogo, dialogo);
}

void Personaje::setPista(char* pista)
{
    this->pista = new char[strlen(pista)+1];
    strcpy(this->pista, pista);
}

void Personaje::setAcertijo(Acertijo* acertijo)
{
    this->acertijo = acertijo;
}

void Personaje::setObjeto(const char* objeto)
{
    this->objeto = new char[strlen(objeto)+1];
    strcpy(this->objeto, objeto);
}

void Personaje::imprimirPersonaje()
{
    cout<<"[Nombre: "<<this->nombre<<", Dialogo: "<<this->dialogo<<", Pista: "<<this->pista<<", Objeto: "<<(objeto ? objeto : "NULL")<<endl;
    (*this->acertijo).imprimirAcertijo();
    cout<<"]"<<endl;
}

char* Personaje::convertirAChar() {
    const char* separador = "#P#";
    char* textoAcertijo = acertijo->convertirAChar();
    const char* objetoTexto = (objeto != nullptr) ? objeto : "";

    int totalLen = strlen(nombre) + strlen(separador)
                 + strlen(dialogo) + strlen(separador)
                 + strlen(pista) + strlen(separador)
                 + strlen(textoAcertijo) + strlen(separador)
                 + strlen(objetoTexto) + 1;

    char* resultado = new char[totalLen];
    strcpy(resultado, nombre);
    strcat(resultado, separador);
    strcat(resultado, dialogo);
    strcat(resultado, separador);
    strcat(resultado, pista);
    strcat(resultado, separador);
    strcat(resultado, textoAcertijo);
    strcat(resultado, separador);
    strcat(resultado, objetoTexto);

    delete[] textoAcertijo;
    return resultado;
}

Personaje::Personaje(const char* linea) {
    const char* separador = "#P#";
    const int MAX_PARTES = 5;
    char* partes[MAX_PARTES] = {nullptr};

    const char* inicio = linea;
    int i = 0;

    while (i < MAX_PARTES - 1) {
        const char* pos = strstr(inicio, separador);
        if (!pos) break;

        int len = pos - inicio;
        partes[i] = new char[len + 1];
        strncpy(partes[i], inicio, len);
        partes[i][len] = '\0';

        inicio = pos + strlen(separador);
        ++i;
    }

    // Última parte (si hay), o nullptr si cadena acabó antes
    if (*inicio != '\0') {
        partes[i] = strdup(inicio);
        ++i;
    }

    // Asignar campos
    nombre   = (partes[0]) ? strdup(partes[0]) : strdup("");
    dialogo  = (partes[1]) ? strdup(partes[1]) : strdup("");
    pista    = (partes[2]) ? strdup(partes[2]) : strdup("");
    acertijo = new Acertijo((partes[3]) ? partes[3] : "#A#");
    objeto   = (i >= 5 && partes[4] && partes[4][0] != '\0') ? strdup(partes[4]) : nullptr;

    // Limpiar partes
    for (int j = 0; j < i; ++j) delete[] partes[j];
}