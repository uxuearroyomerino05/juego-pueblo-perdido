#include "acertijo_Struct.h"
#ifndef _PERSONAJE_Struct_H_ 
#define _PERSONAJE_Struct_H_

    typedef struct {
        char* nombre;
        char* dialogo;
        char* pista;
        Acertijo_Struct acertijo;
        char* objeto;
    }  Personaje_Struct;

    void imprimirPersonaje(Personaje_Struct personaje);

#endif