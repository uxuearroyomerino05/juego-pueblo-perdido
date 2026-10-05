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
        Acertijo(const char* pregunta, const char* respuesta);
        Acertijo(Acertijo* acertijo);
        ~Acertijo();
        char* getPregunta();
        char* getRespuesta();
        void setPregunta(char* pregunta);
        void setRespuesta(char* respuesta);
        void imprimirAcertijo();
        char* convertirAChar();
        Acertijo(const char* linea);
    };

}

#endif
