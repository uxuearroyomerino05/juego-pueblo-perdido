#ifndef _JUGADOR_H_ 
#define _JUGADOR_H_

namespace Usuario
{    

    class Jugador
    {
    private:
        char* nombre;
        char* contrasena;
        char** objetos;
        int cantObjetos;

    public:
        Jugador();
        Jugador(const char* nombre, const char* contrasena);
        ~Jugador();
        char* getNombre();
        char* getContrasena();
        char** getObjetos();
        int getCantObjetos();
        void setNombre(char* nombre);
        void setContrasena(char* contrasena);
        void toStringJugador();
        void anadirObjeto(const char* objeto);
        char* getObjeto(int index);
        char* convertirAChar();
        Jugador(const char* linea);
    };

}

#endif
