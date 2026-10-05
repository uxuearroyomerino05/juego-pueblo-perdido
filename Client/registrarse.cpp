#include "registrarse.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QScreen>
#include <QFont>
#include <winsock2.h>
#include "protocoloMensages.h"
#include <iostream>

#include "jugador.h"
using namespace Usuario;

Registrase::Registrase(const SOCKET s, QWidget* parent)
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

    setWindowTitle("Registrarse");
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titulo = new QLabel("Registrarse");
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
    connect(botonSiguiente, &QPushButton::clicked, this, &Registrase::verificarUsuario);

    lblContrasena = new QLabel("Contraseña:");
    lblContrasena->setFont(fontTexto);
    lblContrasena->hide();
    layout->addWidget(lblContrasena);

    QHBoxLayout* layoutContrasena = new QHBoxLayout();

    campoContrasena = new QLineEdit();
    campoContrasena->setFont(fontTexto);
    campoContrasena->setEchoMode(QLineEdit::Password);
    campoContrasena->hide();

    botonVerContrasena = new QPushButton("🔓");
    botonVerContrasena->setFont(fontTexto);
    botonVerContrasena->setFixedWidth(40);
    botonVerContrasena->hide();

    layoutContrasena->addWidget(campoContrasena);
    layoutContrasena->addWidget(botonVerContrasena);

    layout->addLayout(layoutContrasena);

    connect(botonVerContrasena, &QPushButton::clicked, this, &Registrase::alternarVisibilidadContrasena);


    botonRegistrar = new QPushButton("Registrar");
    botonRegistrar->setFont(fontBoton);
    botonRegistrar->hide();
    layout->addWidget(botonRegistrar);
    connect(botonRegistrar, &QPushButton::clicked, this, &Registrase::registrarUsuario);
}

void Registrase::verificarUsuario()
{
    QString jugador = campoUsuario->text();

    // TODO: Comprobar si el usuario ya existe en la base de datos
    if (jugador == "Uxue") {
        QMessageBox::warning(this, "Error", "Ese nombre de usuario ya está en uso.");

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

        if(usuario->getNombre() == nullptr) {

            campoUsuario->setEnabled(false);
            botonSiguiente->hide();

            lblContrasena->show();
            campoContrasena->show();
            botonVerContrasena->show();
            botonRegistrar->show();

            delete usuario;
            usuario = new Jugador();
            usuario->setNombre(usuario_cstr);

        } else {
            QMessageBox::warning(this, "Error", "Ese nombre de usuario ya está en uso.");
        }

    }
}

void Registrase::registrarUsuario()
{
    QString contrasena = campoContrasena->text();

    if (!contrasena.isEmpty()) {
        QByteArray byteArray = contrasena.toUtf8();
        char* contrasena_cstr = new char[byteArray.size() + 1];
        strcpy(contrasena_cstr, byteArray.constData());
        usuario->setContrasena(contrasena_cstr);

        usuarioaGuardado(this->usuario);
        //LLAMAR SERVIDOR: INSERT_JUGADOR
        char* recv = nullptr;
        char* comando = new char[11+strlen(this->usuario->convertirAChar())+1];
        strcpy(comando, "INSERT_JUGADOR|");
        strcat(comando, this->usuario->convertirAChar());
        enviarYRecibir(s, comando, &recv);

    delete[] recv;
        accept(); // Cierra la ventana
    } else {
        QMessageBox::warning(this, "Error", "La contraseña no puede estar vacía.");
    }
}

void Registrase::closeEvent(QCloseEvent* event)
{
    QMessageBox::warning(this, "Atención", "Debes completar el registro para continuar.");
    event->ignore();
}

void Registrase::alternarVisibilidadContrasena()
{
    contrasenaVisible = !contrasenaVisible;

    if (contrasenaVisible) {
        campoContrasena->setEchoMode(QLineEdit::Normal);
        botonVerContrasena->setText("🔒");
    } else {
        campoContrasena->setEchoMode(QLineEdit::Password);
        botonVerContrasena->setText("🔓");
    }
}

Registrase::~Registrase(){

}