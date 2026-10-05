#ifndef PANTALLAANADIR_H
#define PANTALLAANADIR_H

#include <QWidget>
#include <QPushButton>
#include "personaje.h"
#include "pueblo.h"
#include "zona.h"
#include <winsock2.h>

using namespace Juego;

class PantallaAnadir : public QWidget {
    Q_OBJECT

public:
    PantallaAnadir(const SOCKET s, QWidget* parent = nullptr);
    ~PantallaAnadir();

private:
    void abrirFormularioPueblo();
    void abrirFormularioZona();
    void abrirFormularioPersonaje();
    void volverAlMenuAdministrador();
    void guardarPersonaje(const char* nombre, const char* dialogo,
                          const char* pista, const char* pregunta,
                          const char* respuesta);
    void guardarZona( const char* nombre, const char* descripcion, Personaje** personajes,
                      const int cantP);
    void guardarPueblo(const char* nombre, const char* introduccion, const char* lugarMisterio,
                       const char* misterio, Zona** zonas, const int cantZonas);

    bool obtenerNombresPueblos(char*** nombresPueblos, int* cantidad);

    QPushButton* btnAnadirPueblo;
    QPushButton* btnAnadirZona;
    QPushButton* btnAnadirPersonaje;
    QPushButton* btnVolver;

    SOCKET s;
    char* nombre = nullptr;
    Pueblo* puebloActual = nullptr;

signals:
    void interaccionFinalizada();

};

#endif // PANTALLAANADIR_H

