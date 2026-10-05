#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <winsock2.h>

#include "jugador.h"
#include "pueblo.h"

using namespace Juego;
using namespace Usuario;

class MenuPrincipal : public QWidget {
    Q_OBJECT

public:
    MenuPrincipal(Jugador* jugador, Pueblo* puebloSeleccionado, const SOCKET s, QWidget *parent = nullptr);
    ~MenuPrincipal();

private:
    Jugador* jugador;
    Pueblo* pueblo;
    SOCKET s;

    QVBoxLayout* layout;

    // Botones principales del menú
    QPushButton* btnInvestigar;
    QPushButton* btnVerObjetos;
    QPushButton* btnResolverMisterio;
    QPushButton* btnVolver;

    // Elementos de introducción
    QLabel* titulo;
    QLabel* introduccionLabel;
    QPushButton* btnSaltarIntro;

    // Métodos funcionales
    void investigarZona();
    void verObjetos();
    void resolverMisterio();
    void volverAlMenu();

    // Métodos nuevos para introducción
    void mostrarTextoGradualmente(QLabel* label, const QString& texto);
    void mostrarMenu();
    QTimer* timer;
};

#endif // MENUPRINCIPAL_H

