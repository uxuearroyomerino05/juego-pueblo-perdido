#include "zona.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace Juego;

Zona::Zona()
{

    this->nombre = nullptr;

    this->descripcion = nullptr;

    this->cantP = 0;
    this->personajes = nullptr;
    

}

Zona::Zona(const char* nombre, const char* descripcion)
{

    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);

    this->descripcion = new char[strlen(descripcion)+1];
    strcpy(this->descripcion, descripcion);

    this->cantP = 0;
    this->personajes = nullptr;

}

Zona::Zona(const char* nombre, const char* descripcion, Personaje** personajes, int cantP)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);

    this->descripcion = new char[strlen(descripcion)+1];
    strcpy(this->descripcion, descripcion);

    this->cantP = cantP;
    this->personajes = new Personaje*[cantP];
    for (int i = 0; i < cantP; ++i) {
        this->personajes[i] = new Personaje(*personajes[i]);
    }

}

Zona::~Zona() {
    delete[] nombre;
    delete[] descripcion;
    for (int i = 0; i < cantP; ++i) {
        delete personajes[i];
    }
    delete[] personajes;
}

char* Zona::getNombre(){
    return this->nombre;
}

char* Zona::getDescripcion(){
    return this->descripcion;
}

Personaje** Zona::getPersonajes(){
    return this->personajes;
}

int Zona::getCantP(){
    return this->cantP;
}

void Zona::setNombre(char* nombre){
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);
}

void Zona::setDescripcion(char* descripcion){

    this->descripcion = new char[strlen(descripcion)+1];
    strcpy(this->descripcion, descripcion);
}

void Zona::setPersonajes(Personaje** personajes, int cantP){
    this->personajes = personajes;
    this->cantP = cantP;
}

void Zona::toStringZona()
{
    cout<<"[Nombre: "<<this->nombre<<", Descripcion: "<<this->descripcion<<endl;

    cout<<"Personajes: "<<endl;

    for(int i = 0; i< this->cantP; i++){
        (*this->personajes[i]).toStringPersonaje();
    }

    cout<<"]"<<endl;
}

void Zona::anadirPersonaje(Personaje* personaje)
{
    // Crear nuevo array con espacio adicional
    Personaje** nuevoArray = new Personaje*[this->cantP + 1];

    // Copiar punteros existentes
    for (int i = 0; i < this->cantP; i++) {
        nuevoArray[i] = this->personajes[i];
    }

    // Añadir el nuevo personaje
    nuevoArray[this->cantP] = personaje;  // O copia profunda si es necesario

    // Liberar el array anterior (no los personajes)
    delete[] this->personajes;

    // Actualizar puntero y contador
    this->personajes = nuevoArray;
    this->cantP++;
}

char* Zona::convertirAChar() {
    const char* separadorZona = "#Z#";
    const char* separadorPersonajes = "%Z%";

    // Convertimos todos los personajes
    int totalPersonajeLen = 0;
    char** personajesTexto = new char*[cantP];

    for (int i = 0; i < cantP; ++i) {
        personajesTexto[i] = personajes[i]->convertirAChar();
        totalPersonajeLen += strlen(personajesTexto[i]);
        if (i < cantP - 1)
            totalPersonajeLen += strlen(separadorPersonajes);
    }

    // Longitud total final
    int totalLen = strlen(nombre) + strlen(separadorZona)
                 + strlen(descripcion) + strlen(separadorZona)
                 + totalPersonajeLen + 1;

    char* resultado = new char[totalLen];
    resultado[0] = '\0';

    strcat(resultado, nombre);
    strcat(resultado, separadorZona);
    strcat(resultado, descripcion);
    strcat(resultado, separadorZona);

    for (int i = 0; i < cantP; ++i) {
        strcat(resultado, personajesTexto[i]);
        if (i < cantP - 1)
            strcat(resultado, separadorPersonajes);
        delete[] personajesTexto[i]; // liberamos los char* de personajes
    }

    delete[] personajesTexto;
    return resultado;
}

Zona::Zona(const char* linea) {
    const char* sepCampo = "#Z#";
    const char* sepPersonajes = "%Z%";
    const int MAX_PARTES = 3;
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

    // Última parte (personajes)
    if (*inicio != '\0') partes[i++] = strdup(inicio);

    nombre = strdup(partes[0] ? partes[0] : "");
    descripcion = strdup(partes[1] ? partes[1] : "");
    personajes = nullptr;
    cantP = 0;

    if (i >= 3 && partes[2] && strlen(partes[2]) > 0) {
        // Separar personajes
        const char* str = partes[2];
        int count = 1;
        for (const char* p = str; (p = strstr(p, sepPersonajes)); p += strlen(sepPersonajes))
            ++count;

        personajes = new Personaje*[count];
        cantP = count;

        char* copia = new char[strlen(str) + 1];
        strcpy(copia, str);

        char* ptr = copia;
        for (int j = 0; j < count; ++j) {
            char* next = strstr(ptr, sepPersonajes);
            if (next) *next = '\0';

            personajes[j] = new Personaje(ptr);
            if (!next) break;
            ptr = next + strlen(sepPersonajes);
        }

        delete[] copia;
    }

    for (int j = 0; j < i; ++j) delete[] partes[j];
}

