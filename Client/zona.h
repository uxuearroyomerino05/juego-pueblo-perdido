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
        Zona(const char* nombre, const char* descripcion, Personaje** personajes, int cantP);
        Zona(Zona* z);
        ~Zona();
        char* getNombre();
        char* getDescripcion();
        Personaje** getPersonajes();
        int getCantP();
        void setNombre(char* nombre);
        void setDescripcion(char* descripcion);
        void setPersonajes(Personaje** personajes, int cantP);
        void imprimirZona();
        Zona(const char* linea);
        char* convertirAChar();
        void eliminarPersonaje(Personaje* p);
        void anadirPersonaje(Personaje* p);
    };

}

#endif
