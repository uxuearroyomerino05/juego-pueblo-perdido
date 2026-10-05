#include "personaje_Struct.h"
#ifndef _ZONA_Struct_H_ 
#define _ZONA_Struct_H_

    typedef struct {
        char* nombre;
        char* descripcion;
        Personaje_Struct* personajes;
        int cantP;
    }  Zona_Struct;

    void imprimirZona(Zona_Struct zona);

#endif