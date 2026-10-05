#ifndef _PERSONAJE_H_ 
#define _PERSONAJE_H_

#include "acertijo.h"

namespace Juego
{

    class Personaje
    {
    private:
        char* nombre;
        char* dialogo;
        char* pista;
        Acertijo* acertijo;
        char* objeto;

    public:
        Personaje(const char* nombre, const char* dialogo, const char* pista, Acertijo* acertijo);
        Personaje(const char* nombre, const char* dialogo, const char* pista, Acertijo* acertijo, const char* objeto);
        Personaje();
        Personaje(Personaje* p);
        ~Personaje();
        char* getNombre();
        char* getDialogo();
        char* getPista();
        Acertijo* getAcertijo();
        char* getObjeto();
        void setNombre(char* nombre);
        void setDialogo(char* dialogo);
        void setPista(char* pista);
        void setAcertijo(Acertijo* acertijo);
        void setObjeto(const char* objeto);
        void imprimirPersonaje();
        char* convertirAChar();
        Personaje(const char* linea);
    };

}

#endif
