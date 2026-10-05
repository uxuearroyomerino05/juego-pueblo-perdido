#ifndef _JUGADOR_Struct_H_ 
#define _JUGADOR_Struct_H_

    typedef struct {
        char* nombre;
        char* contrasena;
        char** objetos;
        int cantObjetos;
    }  Jugador_Struct;

    void imprimirJugador(Jugador_Struct jugador);
    void anadirObjeto_Struct(Jugador_Struct *jugador, char* objeto);

#endif