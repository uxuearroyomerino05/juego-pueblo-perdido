#ifndef _ACERTIJO_H_ 
#define _ACERTIJO_H_

namespace Juego 
{


    class Acertijo
    {
    private:
        char* pregunta;
        char* respuesta;

    public:
        Acertijo();
        Acertijo(const char* pregunta, const char* respuesta);
        ~Acertijo();
        char* getPregunta();
        char* getRespuesta();
        void setPregunta(char* pregunta);
        void setRespuesta(char* respuesta);
        void toStringAcertijo();
        char* convertirAChar();
        Acertijo(const char* linea);
    };

}

#endif
