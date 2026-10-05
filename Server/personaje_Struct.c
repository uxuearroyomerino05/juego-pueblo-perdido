#include <stdio.h>
#include "personaje_Struct.h"
#include "acertijo_Struct.h"


void imprimirPersonaje(Personaje_Struct p)
{
	printf("[Nombre: %s, Dialogo: %s, Pista: %s, Objeto: %s ", p.nombre, p.dialogo, p.pista, p.objeto);
    imprimirAcertijo(p.acertijo);
    printf("]\n");
}