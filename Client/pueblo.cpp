#include "pueblo.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace Juego;

Pueblo::Pueblo(const char* nombre, const char* introduccion, const char* lugarMisterio, const char* misterio, Zona** zonas, int cantZonas)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);

    this->introduccion = new char[strlen(introduccion)+1];
    strcpy(this->introduccion, introduccion);

    this->lugarMisterio = new char[strlen(lugarMisterio)+1];
    strcpy(this->lugarMisterio, lugarMisterio);

    this->misterio = new char[strlen(misterio)+1];
    strcpy(this->misterio, misterio);

    this->cantZonas = cantZonas;
    this->zonas = new Zona*[cantZonas];
    for (int i = 0; i < cantZonas; ++i) {
        this->zonas[i] = new Zona(zonas[i]); 
    }

}

Pueblo::Pueblo(Pueblo* p)
{
    this->nombre = new char[strlen(p->nombre)+1];
    strcpy(this->nombre, p->nombre);

    this->introduccion = new char[strlen(p->introduccion)+1];
    strcpy(this->introduccion, p->introduccion);

    this->lugarMisterio = new char[strlen(p->lugarMisterio)+1];
    strcpy(this->lugarMisterio, p->lugarMisterio);

    this->misterio = new char[strlen(p->misterio)+1];
    strcpy(this->misterio, p->misterio);

    this->cantZonas = p->cantZonas;
    this->zonas = new Zona*[cantZonas];
    for (int i = 0; i < cantZonas; ++i) {
        this->zonas[i] = new Zona(p->zonas[i]); 
    }
}

Pueblo::~Pueblo()
{
    delete[] this->nombre;
    delete[] this->introduccion;
    delete[] this->lugarMisterio;
    delete[] this->misterio;
    delete[] this->zonas;
}

char* Pueblo::getNombre(){
    return this->nombre;
}

char* Pueblo::getIntroduccion(){
    return this->introduccion;
}

char* Pueblo::getLugarMisterio(){
    return this->lugarMisterio;
}

char* Pueblo::getMisterio(){
    return this->misterio;
}

Zona** Pueblo::getZonas(){
    return this->zonas;
}

int Pueblo::getCantZonas(){
    return this->cantZonas;
}

void Pueblo::setNombre(char* nombre){
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);
}

void Pueblo::setIntroduccion(char* introduccion){
    this->introduccion = new char[strlen(introduccion)+1];
    strcpy(this->introduccion, introduccion);
}

void Pueblo::setLugarMisterio(char* lugarMisterio){
    this->lugarMisterio = new char[strlen(lugarMisterio)+1];
    strcpy(this->lugarMisterio, lugarMisterio);
}

void Pueblo::setMisterio(char* misterio){
    this->misterio = new char[strlen(misterio)+1];
    strcpy(this->misterio, misterio);
}

void Pueblo::setZonas(Zona** zonas, int cantZonas){
    this->zonas = zonas;

    this->cantZonas = cantZonas;
}

void Pueblo::imprimirPueblo()
{
    cout<<"[Nombre: "<<this->nombre<<endl;

    cout<<"Zonas: "<<endl;

    for(int i = 0; i< this->cantZonas; i++){
        (*this->zonas[i]).imprimirZona();
    }
    cout<<"]"<<endl;
}

char* Pueblo::convertirAChar() {
    const char* sepProp = "#T#";
    const char* sepZonas = "%T%";

    // Convertimos cada zona
    int totalZonasLen = 0;
    char** zonasTexto = new char*[cantZonas];
    for (int i = 0; i < cantZonas; ++i) {
        zonasTexto[i] = zonas[i]->convertirAChar();
        totalZonasLen += strlen(zonasTexto[i]);
        if (i < cantZonas - 1)
            totalZonasLen += strlen(sepZonas);
    }

    int totalLen = strlen(nombre) + strlen(sepProp)
                 + strlen(introduccion) + strlen(sepProp)
                 + strlen(lugarMisterio) + strlen(sepProp)
                 + strlen(misterio) + strlen(sepProp)
                 + totalZonasLen + 1;

    char* resultado = new char[totalLen];
    resultado[0] = '\0';

    strcat(resultado, nombre);
    strcat(resultado, sepProp);
    strcat(resultado, introduccion);
    strcat(resultado, sepProp);
    strcat(resultado, lugarMisterio);
    strcat(resultado, sepProp);
    strcat(resultado, misterio);
    strcat(resultado, sepProp);

    for (int i = 0; i < cantZonas; ++i) {
        strcat(resultado, zonasTexto[i]);
        if (i < cantZonas - 1)
            strcat(resultado, sepZonas);
        delete[] zonasTexto[i];
    }

    delete[] zonasTexto;
    return resultado;
}

Pueblo::Pueblo(const char* linea) {
    const char* sepCampo = "#T#";
    const char* sepZonas = "%T%";
    const int MAX_PARTES = 5;
    const char* partes[MAX_PARTES] = {nullptr};

    const char* inicio = linea;
    int i = 0;

    while (i < MAX_PARTES - 1) {
        const char* pos = strstr(inicio, sepCampo);
        if (!pos) break;

        int len = pos - inicio;
        char* tmp = new char[len + 1];
        strncpy(tmp, inicio, len);
        tmp[len] = '\0';
        partes[i++] = tmp;

        inicio = pos + strlen(sepCampo);
    }

    // Última parte (zonas)
    if (*inicio != '\0') partes[i++] = strdup(inicio);

    nombre = strdup(partes[0] ? partes[0] : "");
    introduccion = strdup(partes[1] ? partes[1] : "");
    lugarMisterio = strdup(partes[2] ? partes[2] : "");
    misterio = strdup(partes[3] ? partes[3] : "");

    zonas = nullptr;
    cantZonas = 0;

    if (i >= 5 && partes[4] && strlen(partes[4]) > 0) {
        const char* str = partes[4];
        int count = 1;
        for (const char* p = str; (p = strstr(p, sepZonas)); p += strlen(sepZonas))
            ++count;

        zonas = new Zona*[count];
        cantZonas = count;

        char* copia = new char[strlen(str) + 1];
        strcpy(copia, str);

        char* ptr = copia;
        for (int j = 0; j < count; ++j) {
            char* next = strstr(ptr, sepZonas);
            if (next) *next = '\0';

            zonas[j] = new Zona(ptr);
            if (!next) break;
            ptr = next + strlen(sepZonas);
        }

        delete[] copia;
    }

    for (int j = 0; j < i; ++j) delete[] partes[j];
}

void Pueblo::anadirZona(Zona* z)
{
    Zona** aux = new Zona*[cantZonas+1];

    for(int i=0; i<cantZonas; i++){
        aux[i] = zonas[i];
    }

    delete zonas;

    zonas = aux;

    zonas[cantZonas] = z;

    cantZonas++;
}


void Pueblo::eliminarZona(Zona* z)
{
    Zona** aux = new Zona*[cantZonas - 1];
    int j = 0;

    for (int i = 0; i < cantZonas; i++) {
        if (strcmp(zonas[i]->getNombre(), z->getNombre()) != 0) {
            aux[j++] = zonas[i];
        }
    }

    delete[] zonas;
    zonas = aux;
    cantZonas--;
}
