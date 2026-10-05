#ifndef _CONVERSOR_DE_DATOS_H_ 
#define _CONVERSOR_DE_DATOS_H_

#include "jugador.h"
#include "acertijo.h"
#include "personaje.h"
#include "zona.h"
#include "pueblo.h"

extern "C" {
    #include "jugador_Struct.h"
    #include "acertijo_Struct.h"
    #include "personaje_Struct.h"
    #include "zona_Struct.h"
    #include "pueblo_Struct.h"
}

// Estructura C → Clase C++
Usuario::Jugador* convertirAJugadorClase(Jugador_Struct* jugadorStruct);
Juego::Acertijo* convertirAAcertijoClase(Acertijo_Struct* acertijoStruct);
Juego::Personaje* convertirAPersonajeClase(Personaje_Struct* personajeStruct);
Juego::Zona* convertirAZonaClase(Zona_Struct* zonaStruct);
Juego::Pueblo* convertirAPuebloClase(Pueblo_Struct* puebloStruct);

// Clase C++ → Estructura C
Jugador_Struct* convertirAJugadorStruct(Usuario::Jugador* jugador);
Acertijo_Struct* convertirAAcertijoStruct(Juego::Acertijo* acertijo);
Personaje_Struct* convertirAPersonajeStruct(Juego::Personaje* personaje);
Zona_Struct* convertirAZonaStruct(Juego::Zona* zona);
Pueblo_Struct* convertirAPuebloStruct(Juego::Pueblo* pueblo);


#endif