#ifndef PANTALLARESOLVERMISTERIO_H
#define PANTALLARESOLVERMISTERIO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

#include "jugador.h"
#include "pueblo.h"

using namespace Usuario;
using namespace Juego;

class PantallaResolverMisterio : public QWidget {
    Q_OBJECT

public:
    PantallaResolverMisterio(QWidget* parent, Jugador* jugador, Pueblo* pueblo);
    ~PantallaResolverMisterio();

private:
    void comenzarColocacion();
    void colocarSiguienteObjeto();
    void mostrarTextoGradualmente();
    void mostrarMisterio();
    void mostrarResolucion();

    Jugador* jugador;
    Pueblo* pueblo;
    int pasoActual;

    QLabel* labelTexto;
    QPushButton* btnAccion;
    QPushButton* btnVolver;
    QTimer* timer;

    QStringList objetosAColocar;

    QString textoEnProceso;
    int indexTexto;
    QTimer* timerEscritura;

signals:
    void volverAlMenu();
};
#endif // PANTALLARESOLVERMISTERIO_H

