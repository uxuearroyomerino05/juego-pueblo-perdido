#ifndef _PUEBLO_H_ 
#define _PUEBLO_H_

#include "zona.h"

namespace Juego 
{

    class Pueblo
    {
    private:
        char* nombre;
        char* introduccion;
        char* lugarMisterio;
        char* misterio;
        Zona** zonas;
        int cantZonas;

    public:
        Pueblo();
        Pueblo(const char* nombre, const char* introduccion, const char* lugarMisterio, const char* misterio);
        Pueblo(const char* nombre, const char* introduccion, const char* lugarMisterio, const char* misterio, Zona** zonas, int cantZonas);
        ~Pueblo();
        char* getNombre();
        char* getIntroduccion();
        char* getLugarMisterio();
        char* getMisterio();
        Zona** getZonas();
        int getCantZonas();
        void setNombre(char* nombre);
        void setIntroduccion(char* introduccion);
        void setLugarMisterio(char* lugarMisterio);
        void setMisterio(char* misterio);
        void setZonas(Zona** zonas, int cantZonas);
        void toStringPueblo();
        void anadirZona(Zona* zona);
        char* convertirAChar();
        Pueblo(const char* linea);
    };

}

#endif
