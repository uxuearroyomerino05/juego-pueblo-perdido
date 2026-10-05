#ifndef PANTALLAELIMINAR_H
#define PANTALLAELIMINAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <winsock2.h>
#include "pueblo.h"

using namespace Juego;

class PantallaEliminar : public QWidget {
    Q_OBJECT

public:
    PantallaEliminar(const SOCKET s, QWidget* parent = nullptr);
    ~PantallaEliminar();

private:
    void eliminarPueblo();
    void eliminarZona();
    void eliminarPersonaje();
    void volverAlMenu();

    QPushButton* btnEliminarPueblo;
    QPushButton* btnEliminarZona;
    QPushButton* btnEliminarPersonaje;
    QPushButton* btnVolver;

    SOCKET s;

signals:
    void interaccionFinalizada();

};

#endif // PANTALLAELIMINAR_H
