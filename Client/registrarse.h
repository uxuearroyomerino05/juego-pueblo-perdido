#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include <QDialog>
#include "jugador.h"
#include <winsock2.h>

using namespace Usuario;

class QLineEdit;
class QPushButton;
class QLabel;

class Registrase : public QDialog
{
    Q_OBJECT

public:
    Registrase(const SOCKET s, QWidget* parent = nullptr);
    ~Registrase();

protected:
    void closeEvent(QCloseEvent* event) override;

signals:
    void usuarioaGuardado(Jugador* usuario);

private:
    SOCKET s;
    Jugador* usuario;
    
    QLineEdit* campoUsuario;
    QLineEdit* campoContrasena;
    QLabel* lblContrasena;
    QPushButton* botonSiguiente;
    QPushButton* botonRegistrar;
    QPushButton* botonVerContrasena;
    bool contrasenaVisible = false;

    void verificarUsuario();
    void registrarUsuario();
    void alternarVisibilidadContrasena();
};

#endif // REGISTRARSE_H

