#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jugador_Struct.h"


void imprimirJugador(Jugador_Struct j)
{
	printf("[Nombre: %s, Contrasena: %s", j.nombre, j.contrasena);

    if(j.cantObjetos != 0){
        printf(", Objetos: ");

        for(int i = 0; i< (j.cantObjetos-1); i++){
            printf("%s, ", j.objetos[i]);
        }
        printf("%s", j.objetos[j.cantObjetos-1]);
    }
    printf("]\n");
}

void anadirObjeto_Struct(Jugador_Struct *jugador, char* objeto) {
    // Crear nuevo array de punteros
    char** aux = malloc(sizeof(char*) * (jugador->cantObjetos + 1));

    // Copiar strings anteriores
    for (int i = 0; i < jugador->cantObjetos; i++) {
        aux[i] = strdup(jugador->objetos[i]);  // Hacer copia del string
    }

    // Liberar array anterior
    for (int i = 0; i < jugador->cantObjetos; i++) {
        free(jugador->objetos[i]);  // Liberar cada string
    }
    free(jugador->objetos);  // Liberar array de punteros

    // Agregar nuevo objeto (haciendo copia)
    aux[jugador->cantObjetos] = strdup(objeto);

    jugador->objetos = aux;
    jugador->cantObjetos++;
}
