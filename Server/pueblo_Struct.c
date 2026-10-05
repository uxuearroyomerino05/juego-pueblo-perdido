#include <stdio.h>
#include "zona_Struct.h"
#include "pueblo_Struct.h"

void imprimirPueblo(Pueblo_Struct pueblo){

    printf("[Nombre: %s, ", pueblo.nombre);

    printf("Zonas: \n");

    for(int i = 0; i< pueblo.cantZonas; i++){
        imprimirZona(pueblo.zonas[i]);
    }
    printf("]\n");

}