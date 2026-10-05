#include "iniciarSesion.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QScreen>
#include <QFont>
#include "ficheros.h"
#include "protocoloMensages.h"
#include <winsock2.h>
#include <iostream>

#include "jugador.h"
using namespace Usuario;

IniciarSesion::IniciarSesion(const SOCKET s, QWidget* parent)
    : QDialog(parent)
{
    this->s = s;

    // Tamaño proporcional al 50% de la pantalla
    QScreen* screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();
    int width = screenSize.width() * 0.5;
    int height = screenSize.height() * 0.5;
    setFixedSize(width, height);
    move((screenSize.width() - width) / 2, (screenSize.height() - height) / 2);

    // Fuentes proporcionales
    int fontTitleSize = height / 18;
    int fontFieldSize = height / 25;
    int fontButtonSize = height / 28;

    QFont fontTitulo("Georgia", fontTitleSize);
    QFont fontTexto("Arial", fontFieldSize);
    QFont fontBoton("Arial", fontButtonSize);

    //Configuracion ventana
    setWindowTitle("Iniciar Sesión");
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titulo = new QLabel("Inicio de Sesión");
    titulo->setFont(fontTitulo);
    titulo->setAlignment(Qt::AlignCenter);
    layout->addWidget(titulo);

    QLabel* lblUsuario = new QLabel("Nombre de usuario:");
    lblUsuario->setFont(fontTexto);
    layout->addWidget(lblUsuario);

    campoUsuario = new QLineEdit();
    campoUsuario->setFont(fontTexto);
    layout->addWidget(campoUsuario);

    botonSiguiente = new QPushButton("Siguiente");
    botonSiguiente->setFont(fontBoton);
    layout->addWidget(botonSiguiente);
    connect(botonSiguiente, &QPushButton::clicked, this, &IniciarSesion::verificarUsuario);

    etiquetaContrasena = new QLabel("Contraseña:");
    etiquetaContrasena->setFont(fontTexto);
    etiquetaContrasena->hide();
    layout->addWidget(etiquetaContrasena);

    campoContrasena = new QLineEdit();
    campoContrasena->setFont(fontTexto);
    campoContrasena->setEchoMode(QLineEdit::Password);
    campoContrasena->hide();
    layout->addWidget(campoContrasena);

    botonLogin = new QPushButton("Iniciar Sesión");
    botonLogin->setFont(fontBoton);
    botonLogin->hide();
    layout->addWidget(botonLogin);
    connect(botonLogin, &QPushButton::clicked, this, &IniciarSesion::verificarCredenciales);

    botonOlvidoContrasena = new QPushButton("¿Olvidaste tu contraseña?");
    botonOlvidoContrasena->setFont(fontBoton);
    botonOlvidoContrasena->hide();
    layout->addWidget(botonOlvidoContrasena);
    connect(botonOlvidoContrasena, &QPushButton::clicked, this, [this]() {
        QString usuarioIngresado = campoUsuario->text();
        const char* usuarioAdmin = obtener_valor_config("admin_user");

        if (usuarioIngresado == QString::fromUtf8(usuarioAdmin)) {
            // Acción si el usuario ingresado es el admin
            QMessageBox::warning(this, "Error", "No se puede cambiar la contraseña del administrador.");
        } else {
            // Acción por defecto
            this->mostrarRecuperarContrasena();
        }
    });


    campoNuevaContrasena = new QLineEdit();
    campoNuevaContrasena->setFont(fontTexto);
    campoNuevaContrasena->setPlaceholderText("Nueva contraseña");
    campoNuevaContrasena->hide();
    layout->addWidget(campoNuevaContrasena);

    botonActualizarContrasena = new QPushButton("Actualizar contraseña");
    botonActualizarContrasena->setFont(fontBoton);
    botonActualizarContrasena->hide();
    layout->addWidget(botonActualizarContrasena);
    connect(botonActualizarContrasena, &QPushButton::clicked, this, &IniciarSesion::actualizarContrasena);
}

void IniciarSesion::verificarUsuario()
{
    QString jugador = campoUsuario->text();

    // TODO: Verificar si el usuario existe en la base de datos
    if (jugador == obtener_valor_config("admin_user")) {
        mostrarCamposContrasena();
    } else {

        //LLAMAR SERVIDOR: GET_JUGADOR
        QByteArray byteArray = jugador.toUtf8();
        char* usuario_cstr = new char[byteArray.size() + 1];
        strcpy(usuario_cstr, byteArray.constData());

        char* recv = nullptr;
        char* send = new char[12+strlen(usuario_cstr)+1];
        strcpy(send, "GET_JUGADOR|");

        strcat(send, usuario_cstr);
        enviarYRecibir(s, send, &recv);

        this->usuario = new Jugador(recv);

        if(usuario->getNombre() != nullptr) {
            mostrarCamposContrasena();
        } else {
            QMessageBox::warning(this, "Error", "El usuario no existe.");
        }
    }
}

void IniciarSesion::mostrarCamposContrasena()
{
    etiquetaContrasena->show();
    campoContrasena->show();
    botonLogin->show();
    botonOlvidoContrasena->show();
    botonSiguiente->hide();
    campoUsuario->setEnabled(false);
}

void IniciarSesion::verificarCredenciales()
{
    QString jugador = campoUsuario->text();
    QString contrasena = campoContrasena->text();

    QByteArray byteArray = contrasena.toUtf8();
    char* contrasena_cstr = new char[byteArray.size() + 1];
    strcpy(contrasena_cstr, byteArray.constData());

    if (jugador == obtener_valor_config("admin_user") && contrasena == obtener_valor_config("admin_contrasena")) {
        QMessageBox::information(this, "Login", "Bienvenido administrador");
        this->usuario = new Jugador(obtener_valor_config("admin_user"), obtener_valor_config("admin_contrasena"));
        usuarioaGuardado(this->usuario);
        accept();

    } else if (jugador == obtener_valor_config("admin_user") && contrasena != obtener_valor_config("admin_contrasena")) {
        QMessageBox::warning(this, "Error", "Contraseña incorrecta");

    }else if (strcmp(contrasena_cstr,this->usuario->getContrasena())==0) {
        QMessageBox::information(this, "Login", "Inicio sesion exitoso");
        usuarioaGuardado(this->usuario);
        accept();

    } else {
            QMessageBox::warning(this, "Error", "Contraseña incorrecta");
    }
}

void IniciarSesion::mostrarRecuperarContrasena()
{
    campoContrasena->hide();
    botonLogin->hide();

    campoNuevaContrasena->show();
    botonActualizarContrasena->show();
}

void IniciarSesion::actualizarContrasena()
{
    QString nueva = campoNuevaContrasena->text();

    if (!nueva.isEmpty()) {
        QByteArray byteArray = nueva.toUtf8();
        char* contrasena_cstr = new char[byteArray.size() + 1];
        strcpy(contrasena_cstr, byteArray.constData());

        // LLAMAR SERVIDOR: MOD_CONTRASENA
        char* recv = nullptr;
        char* coman = new char[16+strlen(this->usuario->getNombre())+1+strlen(contrasena_cstr)+1];
        strcpy(coman, "MOD_CONTRASENA|");
        strcat(coman, this->usuario->getNombre());
        strcat(coman, "|");
        strcat(coman, contrasena_cstr);
        enviarYRecibir(s, coman, &recv);

        this->usuario->setContrasena(contrasena_cstr);

        QMessageBox::information(this, "Contraseña actualizada", "Tu contraseña ha sido actualizada.");

        // Volvemos a mostrar los campos de login
        campoNuevaContrasena->hide();
        botonActualizarContrasena->hide();

        campoContrasena->clear();
        campoContrasena->show();
        botonLogin->show();
    } else {
        QMessageBox::warning(this, "Error", "Introduce una nueva contraseña.");
    }
}

void IniciarSesion::closeEvent(QCloseEvent* event)
{
    QMessageBox::warning(this, "Atención", "Debes iniciar sesión para continuar.");
    event->ignore();
}

IniciarSesion::~IniciarSesion(){

}