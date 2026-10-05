#include "PantallaMenuAdministrador.h"
#include "PantallaAnadir.h"
#include "menuInicio.h"
#include "pantallaEliminar.h"
#include "pantallaModificar.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QFont>

#include <winsock2.h>

PantallaMenuAdministrador::PantallaMenuAdministrador(const SOCKET s, QWidget* parent) : QWidget(parent) {

    //Pueblos y su cantidad
    this->s=s;

    // Tamaño adaptativo
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();

    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;

    // Crear botones
    btnAnadir = new QPushButton("➕ Añadir", this);
    btnModificar = new QPushButton("✏️ Modificar", this);
    btnEliminar = new QPushButton("🗑️ Eliminar", this);
    btnVolver = new QPushButton("⬅️ Volver", this);

    QList<QPushButton*> botones = {btnAnadir, btnModificar, btnEliminar, btnVolver};
    for (auto* btn : botones) {
        btn->setMinimumHeight(buttonHeight);
        btn->setFont(QFont("Verdana", buttonFontSize));
    }

    // Conexiones
    connect(btnAnadir, &QPushButton::clicked, this, &PantallaMenuAdministrador::abrirPantallaAnadir);
    connect(btnModificar, &QPushButton::clicked, this, &PantallaMenuAdministrador::abrirPantallaModificar);
    connect(btnEliminar, &QPushButton::clicked, this, &PantallaMenuAdministrador::abrirPantallaEliminar);
    connect(btnVolver, &QPushButton::clicked, this, &PantallaMenuAdministrador::volverAlMenu);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(btnAnadir);
    layout->addWidget(btnModificar);
    layout->addWidget(btnEliminar);
    layout->addWidget(btnVolver);

    setLayout(layout);
    showFullScreen();
}

void PantallaMenuAdministrador::abrirPantallaAnadir() {
    PantallaAnadir* pantalla = new PantallaAnadir(this->s);
    pantalla->show();
    this->hide();

    // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
    connect(pantalla, &PantallaAnadir::interaccionFinalizada, this, [=]() {
        this->hide();              // Aseguramos reinicio del estado
        this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
        this->showFullScreen();    // Forzamos pantalla completa
    });
}

void PantallaMenuAdministrador::abrirPantallaModificar(){
    PantallaModificar* pantalla = new PantallaModificar(this->s);
    pantalla->show();
    this->hide();

    // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
    connect(pantalla, &PantallaModificar::interaccionFinalizada, this, [=]() {
        this->hide();              // Aseguramos reinicio del estado
        this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
        this->showFullScreen();    // Forzamos pantalla completa
    });
}

void PantallaMenuAdministrador::abrirPantallaEliminar(){
    PantallaEliminar* pantalla = new PantallaEliminar(this->s);
    pantalla->show();
    this->hide();

    // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
    connect(pantalla, &PantallaEliminar::interaccionFinalizada, this, [=]() {
        this->hide();              // Aseguramos reinicio del estado
        this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
        this->showFullScreen();    // Forzamos pantalla completa
    });
}

void PantallaMenuAdministrador::volverAlMenu(){

    MenuInicio* menu = new MenuInicio(this->s);
    menu->show();
    this->close();
}

PantallaMenuAdministrador::~PantallaMenuAdministrador(){

}