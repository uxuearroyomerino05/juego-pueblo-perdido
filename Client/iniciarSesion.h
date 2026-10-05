#ifndef INICIARSESION_H
#define INICIARSESION_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include <winsock2.h>

#include "jugador.h"
using namespace Usuario;

class IniciarSesion : public QDialog
{
    Q_OBJECT

public:
    IniciarSesion(const SOCKET s, QWidget* parent = nullptr);
    ~IniciarSesion();

protected:
    void closeEvent(QCloseEvent* event) override;

signals:
    void usuarioaGuardado(Jugador* usuario);

private:

    Jugador* usuario;
    SOCKET s;

    QLineEdit* campoUsuario;
    QLineEdit* campoContrasena;
    QLineEdit* campoNuevaContrasena;

    QLabel* etiquetaContrasena;

    QPushButton* botonSiguiente;
    QPushButton* botonLogin;
    QPushButton* botonOlvidoContrasena;
    QPushButton* botonActualizarContrasena;

    void mostrarCamposContrasena();
    void verificarUsuario();
    void verificarCredenciales();
    void mostrarRecuperarContrasena();
    void actualizarContrasena();
};

#endif // INICIARSESION_H
