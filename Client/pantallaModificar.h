#ifndef PANTALLAMODIFICAR_H
#define PANTALLAMODIFICAR_H


#include <QWidget>
#include <QPushButton>
#include <winsock2.h>

#include "pueblo.h"

using namespace Juego;

class PantallaModificar : public QWidget {
    Q_OBJECT

public:
    PantallaModificar(const SOCKET s, QWidget* parent = nullptr);
    ~PantallaModificar();

private:
    void seleccionarPueblo();
    void seleccionarZona();
    void seleccionarPersonaje();
    void volverAlMenuAdministrador();
    void modificarPersonaje(const char* nombre, const char* dialogo,
                          const char* pista, const char* pregunta,
                          const char* respuesta);
    void modificarZona( const char* nombre, const char* descripcion, Personaje** personajes,
                     const int cantP);
    void modificarPueblo(const char* nombre, const char* introduccion, const char* lugarMisterio,
                       const char* misterio, Zona** zonas, const int cantZonas);


    QPushButton* btnModificarPueblo;
    QPushButton* btnModificarZona;
    QPushButton* btnModificarPersonaje;
    QPushButton* btnVolver;

    SOCKET s;
    char* str1;
    char* str2;

signals:
    void interaccionFinalizada();

};

#endif // PANTALLAMODIFICAR_H
