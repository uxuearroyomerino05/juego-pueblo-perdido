#ifndef PANTALLAMENUADMINISTRADOR_H
#define PANTALLAMENUADMINISTRADOR_H

#include <QWidget>
#include <QPushButton>
#include "pueblo.h"
#include <winsock2.h>

using namespace Juego;

class PantallaMenuAdministrador : public QWidget {
    Q_OBJECT

public:
    PantallaMenuAdministrador(const SOCKET s, QWidget* parent = nullptr);
    ~PantallaMenuAdministrador();

private:
    SOCKET s;
    QPushButton* btnAnadir;
    QPushButton* btnModificar;
    QPushButton* btnEliminar;
    QPushButton* btnVolver;

    void abrirPantallaAnadir();
    void abrirPantallaModificar();
    void abrirPantallaEliminar();
    void volverAlMenu();

};

#endif // PANTALLAMENUADMINISTRADOR_H

