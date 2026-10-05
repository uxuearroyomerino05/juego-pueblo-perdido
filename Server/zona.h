#ifndef _ZONA_H_ 
#define _ZONA_H_

#include "personaje.h"

namespace Juego 
{

    class Zona
    {
    private:
        char* nombre;
        char* descripcion;
        Personaje** personajes;
        int cantP;

    public:
        Zona();
        Zona(const char* nombre, const char* descripcion);
        Zona(const char* nombre, const char* descripcion, Personaje** personajes, int cantP);
        ~Zona();
        char* getNombre();
        char* getDescripcion();
        Personaje** getPersonajes();
        int getCantP();
        void setNombre(char* nombre);
        void setDescripcion(char* descripcion);
        void setPersonajes(Personaje** personajes, int cantP);
        void toStringZona();
        void anadirPersonaje(Personaje* personaje);
        char* convertirAChar();
        Zona(const char* linea);
    };

}

#endif
