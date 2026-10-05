#include "zona_Struct.h"
#ifndef _PUEBLO_Struct_H_ 
#define _PUEBLO_Struct_H_

    typedef struct {
        char* nombre;
        char* introduccion;
        char* lugarMisterio;
        char* misterio;
        Zona_Struct* zonas;
        int cantZonas;
    }  Pueblo_Struct;

    void imprimirPueblo(Pueblo_Struct pueblo);

#endif