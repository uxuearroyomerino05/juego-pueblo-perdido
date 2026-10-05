#include "ConversorDatos.h"

#include <string.h>
#include <iostream>

#include "jugador.h"
#include "acertijo.h"
#include "personaje.h"
#include "zona.h"
#include "pueblo.h"

extern "C" {
    #include "jugador_Struct.h"
    #include "acertijo_Struct.h"
    #include "personaje_Struct.h"
    #include "zona_Struct.h"
    #include "pueblo_Struct.h"
}

using namespace Juego;
using namespace Usuario;

Jugador_Struct* convertirAJugadorStruct(Usuario::Jugador* jugador) {
    if (!jugador) return nullptr;

    Jugador_Struct* jugadorStruct = new Jugador_Struct;

    // Copiar nombre
    const char* nombre = jugador->getNombre();
    jugadorStruct->nombre = new char[strlen(nombre) + 1];
    strcpy(jugadorStruct->nombre, nombre);

    // Copiar contraseña
    const char* contrasena = jugador->getContrasena();
    jugadorStruct->contrasena = new char[strlen(contrasena) + 1];
    strcpy(jugadorStruct->contrasena, contrasena);

    // Copiar objetos
    int cantObjetos = jugador->getCantObjetos();
    jugadorStruct->cantObjetos = cantObjetos;
    jugadorStruct->objetos = new char*[cantObjetos];

    for (int i = 0; i < cantObjetos; ++i) {
        const char* obj = jugador->getObjetos()[i];
        jugadorStruct->objetos[i] = new char[strlen(obj) + 1];
        strcpy(jugadorStruct->objetos[i], obj);
    }

    return jugadorStruct;
}

Usuario::Jugador* convertirAJugadorClase(Jugador_Struct* jugadorStruct) {
    if (!jugadorStruct) return nullptr;

    if(!jugadorStruct->nombre){
        Jugador* jugador = new Jugador();
        return jugador;
    }

    Jugador* jugador = new Usuario::Jugador(
        jugadorStruct->nombre,
        jugadorStruct->contrasena
    );

    for (int i = 0; i < jugadorStruct->cantObjetos; ++i) {
        jugador->anadirObjeto(jugadorStruct->objetos[i]);
    }

    return jugador;
}

Acertijo* convertirAAcertijoClase(Acertijo_Struct* acertijoStruct) {
    if (!acertijoStruct) return nullptr;

    Juego::Acertijo* acertijo = new Acertijo(acertijoStruct->pregunta, acertijoStruct->respuesta);

    return acertijo;
}

Acertijo_Struct* convertirAAcertijoStruct(Acertijo* acertijo) {

    if (!acertijo) return nullptr;

    Acertijo_Struct* acertijoStruct = new Acertijo_Struct;
    
    const char* pregunta = acertijo->getPregunta();
    const char* respuesta = acertijo->getRespuesta();

    acertijoStruct->pregunta = new char[strlen(pregunta) + 1];
    strcpy(acertijoStruct->pregunta, pregunta);

    acertijoStruct->respuesta = new char[strlen(respuesta) + 1];
    strcpy(acertijoStruct->respuesta, respuesta);

    return acertijoStruct;
}

Personaje_Struct* convertirAPersonajeStruct(Juego::Personaje* personaje) {
    if (!personaje) return nullptr;

    Personaje_Struct* personajeStruct = new Personaje_Struct;

    // Copiar strings
    const char* nombre = personaje->getNombre();
    personajeStruct->nombre = new char[strlen(nombre) + 1];
    strcpy(personajeStruct->nombre, nombre);

    const char* dialogo = personaje->getDialogo();
    personajeStruct->dialogo = new char[strlen(dialogo) + 1];
    strcpy(personajeStruct->dialogo, dialogo);

    const char* pista = personaje->getPista();
    personajeStruct->pista = new char[strlen(pista) + 1];
    strcpy(personajeStruct->pista, pista);

    char* objeto = personaje->getObjeto();
    if(!objeto){
        personajeStruct->objeto = nullptr;
    } else {
        personajeStruct->objeto = new char[strlen(objeto) + 1];
        strcpy(personajeStruct->objeto, objeto);
    }

    // Convertir el acertijo
    personajeStruct->acertijo = *convertirAAcertijoStruct(personaje->getAcertijo());

    return personajeStruct;
}

Juego::Personaje* convertirAPersonajeClase(Personaje_Struct* personajeStruct) {

    if (!personajeStruct) return nullptr;

    // Crear objeto Acertijo
    Juego::Acertijo* acertijo = convertirAAcertijoClase(&personajeStruct->acertijo);

    //Comprobar objeto 
    char* objeto = personajeStruct->objeto;
    
    // Crear personaje
    Juego::Personaje* personaje = new Juego::Personaje(
        personajeStruct->nombre,
        personajeStruct->dialogo,
        personajeStruct->pista,
        acertijo,
        objeto
    );

    return personaje;
}

Zona_Struct* convertirAZonaStruct(Juego::Zona* zona) {
    if (!zona) return nullptr;

    Zona_Struct* zonaStruct = new Zona_Struct;

    // Copiar nombre
    const char* nombre = zona->getNombre();
    zonaStruct->nombre = new char[strlen(nombre) + 1];
    strcpy(zonaStruct->nombre, nombre);

    // Copiar descripción
    const char* descripcion = zona->getDescripcion();
    zonaStruct->descripcion = new char[strlen(descripcion) + 1];
    strcpy(zonaStruct->descripcion, descripcion);

    // Copiar personajes
    int cantP = zona->getCantP();
    zonaStruct->cantP = cantP;
    zonaStruct->personajes = new Personaje_Struct[cantP];

    for (int i = 0; i < cantP; ++i) {
        Juego::Personaje* personaje = zona->getPersonajes()[i];
        Personaje_Struct* personajeStruct = convertirAPersonajeStruct(personaje);
        zonaStruct->personajes[i] = *personajeStruct;
    }

    return zonaStruct;
}

Zona* convertirAZonaClase(Zona_Struct* zonaStruct) {

    if (!zonaStruct) return nullptr;

    Zona* zona = new Zona(
        zonaStruct->nombre,
        zonaStruct->descripcion
    );

    for (int i = 0; i < zonaStruct->cantP; ++i) {
        Personaje* personaje = convertirAPersonajeClase(&zonaStruct->personajes[i]);
        if (!personaje) {
            std::cout << "Error: personaje "<< i <<" es nullptr"<< std::endl;
            continue;
        }
        zona->anadirPersonaje(personaje);
    }

    return zona;
}

Pueblo_Struct* convertirAPuebloStruct(Juego::Pueblo* pueblo) {
    if (!pueblo) return nullptr;

    Pueblo_Struct* puebloStruct = new Pueblo_Struct;

    // Copiar nombre
    const char* nombre = pueblo->getNombre();
    puebloStruct->nombre = new char[strlen(nombre) + 1];
    strcpy(puebloStruct->nombre, nombre);

    // Copiar introducción
    const char* introduccion = pueblo->getIntroduccion();
    puebloStruct->introduccion = new char[strlen(introduccion) + 1];
    strcpy(puebloStruct->introduccion, introduccion);

    // Copiar lugar del misterio
    const char* lugarMisterio = pueblo->getLugarMisterio();
    puebloStruct->lugarMisterio = new char[strlen(lugarMisterio) + 1];
    strcpy(puebloStruct->lugarMisterio, lugarMisterio);

    // Copiar misterio
    const char* misterio = pueblo->getMisterio();
    puebloStruct->misterio = new char[strlen(misterio) + 1];
    strcpy(puebloStruct->misterio, misterio);

    // Copiar zonas
    int cantZonas = pueblo->getCantZonas();
    puebloStruct->cantZonas = cantZonas;
    puebloStruct->zonas = new Zona_Struct[cantZonas];

    for (int i = 0; i < cantZonas; ++i) {
        Juego::Zona* zona = pueblo->getZonas()[i];
        Zona_Struct* zonaStruct = convertirAZonaStruct(zona);
        puebloStruct->zonas[i] = *zonaStruct;
    }

    return puebloStruct;
}

Pueblo* convertirAPuebloClase(Pueblo_Struct* puebloStruct) {

    if (!puebloStruct) return nullptr;

    Pueblo* pueblo = new Juego::Pueblo(
        puebloStruct->nombre,
        puebloStruct->introduccion,
        puebloStruct->lugarMisterio,
        puebloStruct->misterio
    );

    for (int i = 0; i < puebloStruct->cantZonas; ++i) {
        Zona* zona = convertirAZonaClase(&puebloStruct->zonas[i]);
        if (!zona) {
            std::cout << "Error: zona "<< i <<" es nullptr"<< std::endl;
            continue;
        }
        pueblo->anadirZona(zona);

    }

    return pueblo;
}
