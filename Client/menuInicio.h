#ifndef MENUINICIO_H
#define MENUINICIO_H

#include "acertijo.h"
#include "jugador.h"
#include "personaje.h"
#include "pueblo.h"
#include "zona.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <winsock2.h>

using namespace Juego;
using namespace Usuario;

QT_BEGIN_NAMESPACE
namespace Ui { class MenuInicio; }
QT_END_NAMESPACE

class MenuInicio : public QWidget
{
    Q_OBJECT

public:
    explicit MenuInicio( const SOCKET s, QWidget *parent = nullptr);
    ~MenuInicio();

protected:
    void resizeEvent(QResizeEvent *event) override; // Método para manejar el redimensionamiento

private slots:
    void guardarUsuario(Jugador* usuario);

private:
    QLabel *titulo;
    QPushButton *btnIniciarSesion;
    QPushButton *btnRegistrarse;
    QPushButton *btnSalir;

    SOCKET s;
    Jugador *usuario;

    void empezarJuego();

};


#endif // MENUINICIO_H
