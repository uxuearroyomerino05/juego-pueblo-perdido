#include "PantallaAnadir.h"
#include "PantallaAnadirPersonaje.h"
#include "PantallaAnadirZona.h"
#include "acertijo.h"
#include "pantallaAnadirPueblo.h"
#include "personaje.h"
#include "pueblo.h"
#include <QApplication>
#include <QScreen>
#include "seleccionPantalla.h"
#include "zona.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <winsock2.h>
#include "protocoloMensages.h"
#include <iostream>

using namespace Juego;

PantallaAnadir::PantallaAnadir(const SOCKET s, QWidget* parent)
    : QWidget(parent) {

    this->s=s;


    // Obtener dimensiones para escalado
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();

    // Tamaños relativos (ajustables)
    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;

    //Botones
    btnAnadirPueblo = new QPushButton("🏘️Añadir Pueblo", this);
    btnAnadirZona = new QPushButton("🗺️Añadir Zona", this);
    btnAnadirPersonaje = new QPushButton("👤Añadir Personaje", this);
    btnVolver = new QPushButton("⬅️ Volver", this);

    QFont fuente("Verdana", buttonFontSize);
    btnAnadirPueblo->setFont(fuente);
    btnAnadirPueblo->setMinimumHeight(buttonHeight);
    btnAnadirZona->setFont(fuente);
    btnAnadirZona->setMinimumHeight(buttonHeight);
    btnAnadirPersonaje->setFont(fuente);
    btnAnadirPersonaje->setMinimumHeight(buttonHeight);
    btnVolver->setFont(fuente);
    btnVolver->setMinimumHeight(buttonHeight);

    connect(btnAnadirPueblo, &QPushButton::clicked, this, &PantallaAnadir::abrirFormularioPueblo);
    connect(btnAnadirZona, &QPushButton::clicked, this, &PantallaAnadir::abrirFormularioZona);
    connect(btnAnadirPersonaje, &QPushButton::clicked, this, &PantallaAnadir::abrirFormularioPersonaje);
    connect(btnVolver, &QPushButton::clicked, this, &PantallaAnadir::volverAlMenuAdministrador);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(btnAnadirPueblo);
    layout->addWidget(btnAnadirZona);
    layout->addWidget(btnAnadirPersonaje);
    layout->addWidget(btnVolver);
    setLayout(layout);

    showFullScreen();
}

void PantallaAnadir::abrirFormularioPueblo() {

    //Abriri pantalla creacion pueblo
    PantallaAnadirPueblo* pantallaPueblo = new PantallaAnadirPueblo();
    pantallaPueblo->setAttribute(Qt::WA_DeleteOnClose);
    connect(pantallaPueblo, &PantallaAnadirPueblo::puebloGuardado, this, &PantallaAnadir::guardarPueblo);
    pantallaPueblo->show();

    this->hide();

    // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
    connect(pantallaPueblo, &PantallaAnadirPueblo::volverAlMenu, this, [=]() {
        this->hide();              // Aseguramos reinicio del estado
        this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
        this->showFullScreen();    // Forzamos pantalla completa
    });

}

void PantallaAnadir::abrirFormularioZona() {
    // LLAMAR SERVIDOR: GET_NOMBRES_PUEBLOS
    char** nombresPueblos = nullptr;
    int cantidad = 0;

    if (!obtenerNombresPueblos(&nombresPueblos, &cantidad)) {
        QMessageBox::critical(this, "Error", "No se pudieron obtener los pueblos.");
        return;
    }

    SeleccionPantalla* seleccionPueblo = new SeleccionPantalla("pueblo", nombresPueblos, cantidad);
    connect(seleccionPueblo, &SeleccionPantalla::seleccionada, this, [=](int indicePueblo) {

        //LLAMAR SERVIDOR: GET_PUEBLO
        char* recv = nullptr;
        char* comando = new char[11+strlen(nombresPueblos[indicePueblo])+1];
        strcpy(comando, "GET_PUEBLO|");
        strcat(comando, nombresPueblos[indicePueblo]);
        enviarYRecibir(s, comando, &recv);

        if (puebloActual) delete puebloActual;
        puebloActual = new Pueblo(recv);
        if (this->nombre != nullptr) delete[] this->nombre;
        this->nombre = new char[strlen(puebloActual->getNombre())+1];
        strcpy(this->nombre, puebloActual->getNombre());
        seleccionPueblo->close();

        //Limpiar memoria
        delete[] recv;
        delete[] comando;

        //Abriri pantalla anadir zona
        PantallaAnadirZona* pantallaZona = new PantallaAnadirZona();
        pantallaZona->setAttribute(Qt::WA_DeleteOnClose);
        connect(pantallaZona, &PantallaAnadirZona::zonaGuardado, this, &PantallaAnadir::guardarZona);
        pantallaZona->show();


        this->hide();

        // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
        connect(pantallaZona, &PantallaAnadirZona::volverAlMenu, this, [=]() {
            this->hide();              // Aseguramos reinicio del estado
            this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
            this->showFullScreen();    // Forzamos pantalla completa
        });

    });
    seleccionPueblo->show();
    /*
    for (int i = 0; i < cantidad; ++i) {
        delete[] nombresPueblos[i];
    } */
    //delete[] nombresPueblos;

}

void PantallaAnadir::abrirFormularioPersonaje() {

    // LLAMAR SERVIDOR: GET_NOMBRES_PUEBLOS
    char** nombresPueblos = nullptr;
    int cantidad = 0;

    if (!obtenerNombresPueblos(&nombresPueblos, &cantidad)) {
        QMessageBox::critical(this, "Error", "No se pudieron obtener los pueblos.");
        return;
    }

    SeleccionPantalla* seleccionPueblo = new SeleccionPantalla("pueblo", nombresPueblos, cantidad);
    connect(seleccionPueblo, &SeleccionPantalla::seleccionada, this, [=](int indicePueblo) {

        //LLAMAR SERVIDOR: GET_PUEBLO
        char* recv = nullptr;
        char* comando = new char[11+strlen(nombresPueblos[indicePueblo])+1];
        strcpy(comando, "GET_PUEBLO|");
        strcat(comando, nombresPueblos[indicePueblo]);
        enviarYRecibir(s, comando, &recv);

        if (puebloActual) delete puebloActual;
        puebloActual = new Pueblo(recv);
        seleccionPueblo->close();

        //Limpiar memoria
        delete[] recv;
        delete[] comando;

        // Pantalla selección de zona
        int cantidadZonas = puebloActual->getCantZonas();
        char** nombresZonas = new char*[cantidadZonas];
        for (int i = 0; i < cantidadZonas; ++i) {
            const char* nombreZona = puebloActual->getZonas()[i]->getNombre();
            nombresZonas[i] = new char[strlen(nombreZona) + 1];
            strcpy(nombresZonas[i], nombreZona);
        }

        SeleccionPantalla* seleccionZona = new SeleccionPantalla("zona", nombresZonas, cantidadZonas);
        connect(seleccionZona, &SeleccionPantalla::seleccionada, this, [=](int indiceZona) {

            Zona* zonaSeleccionada =  puebloActual->getZonas()[indiceZona];
            if (this->nombre != nullptr) delete[] this->nombre;
            this->nombre = new char[strlen(zonaSeleccionada->getNombre())+1];
            strcpy(this->nombre, zonaSeleccionada->getNombre());
            seleccionZona->close();

            PantallaAnadirPersonaje* pantallaPersonaje = new PantallaAnadirPersonaje();
            pantallaPersonaje->setAttribute(Qt::WA_DeleteOnClose);
            connect(pantallaPersonaje, &::PantallaAnadirPersonaje::personajeGuardado, this, &PantallaAnadir::guardarPersonaje);
            pantallaPersonaje->show();

            this->hide();

            // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
            connect(pantallaPersonaje, &PantallaAnadirPersonaje::volverAlMenu, this, [=]() {
                this->hide();              // Aseguramos reinicio del estado
                this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
                this->showFullScreen();    // Forzamos pantalla completa
            });

        });
        seleccionZona->show();
        seleccionPueblo->close();

    });
    seleccionPueblo->show();

}

void PantallaAnadir::volverAlMenuAdministrador() {
    emit interaccionFinalizada();
    this->close();
}

void PantallaAnadir::guardarPersonaje(const char* nombre, const char* dialogo,
                                      const char* pista, const char* pregunta,
                                      const char* respuesta) {
    Acertijo* acertijo = new Acertijo(pregunta, respuesta);
    Personaje* personaje = new Personaje(nombre, dialogo, pista, acertijo);

    //LLAMAR SERVIDOR: INSERT_PERSONAJE
    char* recv = nullptr;
    char* personajeStr = personaje->convertirAChar();  // cambiar a char*, no const char*

    char* comando = new char[18 + strlen(personajeStr) + 1 + strlen(this->nombre) + 1];
    strcpy(comando, "INSERT_PERSONAJE|");
    strcat(comando, personajeStr);
    strcat(comando, "|");
    strcat(comando, this->nombre);

    enviarYRecibir(s, comando, &recv);

    delete[] comando;
    if (recv) delete[] recv;
    delete[] personajeStr; 
    delete acertijo;
    delete personaje;

    this->hide();
    this->setWindowState(Qt::WindowNoState);
    this->showFullScreen();

}

void PantallaAnadir::guardarZona(const char* nombre, const char* descripcion, Personaje** personajes,
                                 const int cantP) {
    Zona* zona = new Zona(nombre, descripcion, personajes, cantP);

    //LLAMAR SERVIDOR: INSERT_ZONA
    char* recv = nullptr;
    char* zonaStr = zona->convertirAChar();  // cambiar a char*, no const char*

    char* comando = new char[13 + strlen(zonaStr) + 1 + strlen(this->nombre) + 1];
    strcpy(comando, "INSERT_ZONA|");
    strcat(comando, zonaStr);
    strcat(comando, "|");
    strcat(comando, this->nombre);

    enviarYRecibir(s, comando, &recv);

    delete[] comando;
    if (recv) delete[] recv;
    delete[] zonaStr;  
    delete zona;

    this->hide();              
    this->setWindowState(Qt::WindowNoState);  
    this->showFullScreen();    
}


void PantallaAnadir::guardarPueblo(const char* nombre, const char* introduccion, const char* lugarMisterio,
                                   const char* misterio, Zona** zonas, const int cantZonas) {

    Pueblo* pueblo = new Pueblo(nombre, introduccion, lugarMisterio, misterio, zonas, cantZonas);

    //LLAMAR SERVIDOR: INSERT_PUEBLO
    char* recv = nullptr;
    char* comando = new char[11+strlen(pueblo->convertirAChar())+1];
    strcpy(comando, "INSERT_PUEBLO|");
    strcat(comando, pueblo->convertirAChar());
    enviarYRecibir(s, comando, &recv);

    delete[] comando;
    if (recv) delete[] recv;
    delete pueblo;

    this->hide();              // Aseguramos reinicio del estado
    this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
    this->showFullScreen();    // Forzamos pantalla completa
}

bool PantallaAnadir::obtenerNombresPueblos(char*** nombresPueblos, int* cantidad) {
    char* recv = nullptr;
    const char* coman = "GET_NOMBRES_PUEBLO";
    enviarYRecibir(s, const_cast<char*>(coman), &recv);

    if (!recv || strlen(recv) == 0) {
        return false;
    }

    *cantidad = 0;
    *nombresPueblos = nullptr;

    char* token = strtok(recv, "%L%");
    while (token != nullptr) {
        // Reservar nuevo array con espacio adicional
        char** aux = new char*[*cantidad + 1];

        // Copiar punteros anteriores
        for (int i = 0; i < *cantidad; i++) {
            aux[i] = (*nombresPueblos)[i];
        }

        // Agregar nuevo token
        aux[*cantidad] = new char[strlen(token) + 1];
        strcpy(aux[*cantidad], token);
        (*cantidad)++;

        // Liberar array anterior
        delete[] *nombresPueblos;
        *nombresPueblos = aux;

        token = strtok(nullptr, "%L%");
    }

    if (recv) delete[] recv;
    return true;
}


PantallaAnadir::~PantallaAnadir(){
    if (nombre) delete[] nombre;
    if (puebloActual) delete puebloActual;
}