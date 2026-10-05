#include "zona_Struct.h"
#include "personaje_Struct.h"
#include "acertijo_Struct.h"
#include "jugador_Struct.h"
#include "pueblo_Struct.h"

#ifndef _BASEDEDATOS_H_ 
#define _BASEDEDATOS_H_

    void insertarJugador(Jugador_Struct jugador);
    void insertarObjetoJugador(char* nombreJugador, char* nombreObjeto, char* nombrePueblo);
    void insertarPueblo(Pueblo_Struct pueblo);
    void insertarZona(Zona_Struct zona, char* nombrePueblo); 
    void insertarPersonaje(Personaje_Struct personaje, const char* nombreZona); 
    void insertarAcertijo(Acertijo_Struct acertijo); 
    void insertarObjeto(char* objeto, const char* nombreZona); 
    void modificarPueblo(Pueblo_Struct pueblo, Pueblo_Struct puebloAnterior); 
    void modificarZona(Zona_Struct zona, Zona_Struct zonaAnterior); 
    void modificarPersonaje(Personaje_Struct personaje, Personaje_Struct personajeAnterior);  
    void modificarAcertijo(Acertijo_Struct acertijo, char* preguntaAnterior);  
    void modificarObjeto(char* objeto, int idZona);  
    void eliminarPueblo(Pueblo_Struct pueblo); 
    void eliminarZona(Zona_Struct zona);  
    void eliminarPersonaje(Personaje_Struct personaje);  
    void eliminarAcertijo(Acertijo_Struct acertijo);  
    void eliminarObjeto(char* o);
    Zona_Struct* obtenerZonas(int* cantZ, char* idPueblo);
    Personaje_Struct* obtenerPersonajesPorIdZona(int id, int* cantP);
    Acertijo_Struct obtenerAcertijoPorId(int id);
    Jugador_Struct obetenerJugadorPorNombre(char* nombre);
    char* obtenerObjetoPorId(int id);
    void obtenerObjetosPorIdJugadorPorIdPueblo(Jugador_Struct *j, char* idPueblo);
    int obtnerIdObjetoPorNombre(char* nombre);
    void modificarContrasena(char* nombreJugador, char* nueva_contrasena);
    char** obtenerNombresPueblo(int* cantP);
    Pueblo_Struct obtenerPueblo(char* nombre);
    int obtnerIdAcertijoPorPregunta(char* pregunta); 
    int obtnerIdZonaPorNombre(const char* nombreZona); 
    char* obtenerObjetoPorIdPorIdZona(int id, int idZona);

#endif