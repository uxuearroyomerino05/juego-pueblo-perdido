#include <stdio.h>
#include "zona_Struct.h"
#include "personaje_Struct.h"


void imprimirZona(Zona_Struct z)
{
	printf("[Nombre: %s, Descripcion: %s", z.nombre, z.descripcion);

    if (z.cantP > 0) {
        printf("Personajes: \n");

        for(int i = 0; i< z.cantP; i++){
            imprimirPersonaje(z.personajes[i]);
        }
    }
    printf("]\n");
}