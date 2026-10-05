#include "pantallaModificar.h"
#include "PantallaAnadirPersonaje.h"
#include "SeleccionPantalla.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QFont>
#include <winsock2.h>
#include "protocoloMensages.h"
#include "pantallaAnadirPueblo.h"
#include "pantallaAnadirZona.h"
#include "pueblo.h"
#include "zona.h"
#include "personaje.h"

using namespace Juego;

PantallaModificar::PantallaModificar(const SOCKET s, QWidget* parent)
    : QWidget(parent) {

    this->s=s;

    // Obtener dimensiones para escalado
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();

    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;

    // Crear botones
    btnModificarPueblo = new QPushButton("🏘️ Modificar Pueblo", this);
    btnModificarZona = new QPushButton("🗺️ Modificar Zona", this);
    btnModificarPersonaje = new QPushButton("👤 Modificar Personaje", this);
    btnVolver = new QPushButton("⬅️ Volver", this);

    QFont fuente("Verdana", buttonFontSize);
    btnModificarPueblo->setFont(fuente);
    btnModificarPueblo->setMinimumHeight(buttonHeight);
    btnModificarZona->setFont(fuente);
    btnModificarZona->setMinimumHeight(buttonHeight);
    btnModificarPersonaje->setFont(fuente);
    btnModificarPersonaje->setMinimumHeight(buttonHeight);
    btnVolver->setFont(fuente);
    btnVolver->setMinimumHeight(buttonHeight);

    // Conexiones
    connect(btnModificarPueblo, &QPushButton::clicked, this, &PantallaModificar::seleccionarPueblo);
    connect(btnModificarZona, &QPushButton::clicked, this, &PantallaModificar::seleccionarZona);
    connect(btnModificarPersonaje, &QPushButton::clicked, this, &PantallaModificar::seleccionarPersonaje);
    connect(btnVolver, &QPushButton::clicked, this, &PantallaModificar::volverAlMenuAdministrador);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(btnModificarPueblo);
    layout->addWidget(btnModificarZona);
    layout->addWidget(btnModificarPersonaje);
    layout->addWidget(btnVolver);
    setLayout(layout);

    showFullScreen();
}

void PantallaModificar::volverAlMenuAdministrador() {
    emit interaccionFinalizada();
    this->close();
}


void PantallaModificar::seleccionarPueblo() {
    // LLAMAR SERVIDOR: GET_NOMBRES_PUEBLOS
    char* recv = nullptr;
    char coman[] = "GET_NOMBRES_PUEBLO";
    enviarYRecibir(s, coman, &recv);

    char** nombresPueblos = nullptr;
    int cantidad = 0;

    if (recv != nullptr) {
        char* token = strtok(recv, "%L%");
        while (token != nullptr) {
            // Reservar nuevo array con espacio adicional
            char** aux = new char*[cantidad + 1];
            
            // Copiar punteros anteriores
            for (int i = 0; i < cantidad; i++) {
                aux[i] = nombresPueblos[i];
            }

            // Agregar nuevo token
            aux[cantidad] = new char[strlen(token) + 1];
            strcpy(aux[cantidad], token);
            cantidad++;

            // Liberar array anterior
            delete nombresPueblos;
            nombresPueblos = aux;

            // Obtener siguiente token
            token = strtok(nullptr, "%L%");
        }
    }

    delete[] recv;

    SeleccionPantalla* seleccionPueblo = new SeleccionPantalla("pueblo", nombresPueblos, cantidad);
    connect(seleccionPueblo, &SeleccionPantalla::seleccionada, this, [=](int indicePueblo) {

        //LLAMAR SERVIDOR: GET_PUEBLO
        char* recv = nullptr;
        char* comando = new char[11+strlen(nombresPueblos[indicePueblo])+1];
        strcpy(comando, "GET_PUEBLO|");
        strcat(comando, nombresPueblos[indicePueblo]);
        enviarYRecibir(s, comando, &recv);

        Pueblo* pueblo = new Pueblo(recv);
        seleccionPueblo->close();

        delete[] recv;
        delete[] comando;

        //Esconder pantalla actual
        this->hide();

        str1 = new char[strlen(pueblo->convertirAChar())+1];
        strcpy(str1, pueblo->convertirAChar());

        //Abriri pantalla edicion pueblo
        PantallaAnadirPueblo* pantallaPueblo = new PantallaAnadirPueblo(pueblo);
        connect(pantallaPueblo, &PantallaAnadirPueblo::puebloGuardado, this, &PantallaModificar::modificarPueblo);
        pantallaPueblo->show();

        this->hide();

        // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
        connect(pantallaPueblo, &PantallaAnadirPueblo::volverAlMenu, this, [=]() {
            this->hide();              // Aseguramos reinicio del estado
            this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
            this->showFullScreen();    // Forzamos pantalla completa
        });


    });
    seleccionPueblo->show();
}


void PantallaModificar::seleccionarZona() {
    // LLAMAR SERVIDOR: GET_NOMBRES_PUEBLOS
    char* recv = nullptr;
    char coman[] = "GET_NOMBRES_PUEBLO";
    enviarYRecibir(s, coman, &recv);

    char** nombresPueblos = nullptr;
    int cantidad = 0;

    if (recv != nullptr) {
        char* token = strtok(recv, "%L%");
        while (token != nullptr) {
            // Reservar nuevo array con espacio adicional
            char** aux = new char*[cantidad + 1];
            
            // Copiar punteros anteriores
            for (int i = 0; i < cantidad; i++) {
                aux[i] = nombresPueblos[i];
            }

            // Agregar nuevo token
            aux[cantidad] = new char[strlen(token) + 1];
            strcpy(aux[cantidad], token);
            cantidad++;

            // Liberar array anterior
            delete nombresPueblos;
            nombresPueblos = aux;

            // Obtener siguiente token
            token = strtok(nullptr, "%L%");
        }
    }

    delete[] recv;

    SeleccionPantalla* seleccionPueblo = new SeleccionPantalla("pueblo", nombresPueblos, cantidad);

    connect(seleccionPueblo, &SeleccionPantalla::seleccionada, this, [=](int indicePueblo) {

        //LLAMAR SERVIDOR: GET_PUEBLO
        char* recv = nullptr;
        char* comando = new char[11+strlen(nombresPueblos[indicePueblo])+1];
        strcpy(comando, "GET_PUEBLO|");
        strcat(comando, nombresPueblos[indicePueblo]);
        enviarYRecibir(s, comando, &recv);

        Pueblo* pueblo = new Pueblo(recv);
        seleccionPueblo->close();

        delete[] recv;
        delete[] comando;

        // Pantalla selección de zona
        int cantidadZonas = pueblo->getCantZonas();
        char** nombresZonas = new char*[cantidadZonas];
        for (int i = 0; i < cantidadZonas; ++i) {
            const char* nombreZona = pueblo->getZonas()[i]->getNombre();
            nombresZonas[i] = new char[strlen(nombreZona) + 1];
            strcpy(nombresZonas[i], nombreZona);
        }

        SeleccionPantalla* seleccionZona = new SeleccionPantalla("zona", nombresZonas, cantidadZonas);
        connect(seleccionZona, &SeleccionPantalla::seleccionada, this, [=](int indiceZona) {

            Zona* zonaSeleccionada =  pueblo->getZonas()[indiceZona];
            seleccionZona->close();
            str1 = new char[strlen(zonaSeleccionada->convertirAChar())+1];
            strcpy(str1, zonaSeleccionada->convertirAChar());

            PantallaAnadirZona* pantallaZona = new PantallaAnadirZona(zonaSeleccionada);
            connect(pantallaZona, &PantallaAnadirZona::zonaGuardado, this, &PantallaModificar::modificarZona);
            pantallaZona->show();


            this->hide();

            // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
            connect(pantallaZona, &PantallaAnadirZona::volverAlMenu, this, [=]() {
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

void PantallaModificar::seleccionarPersonaje() {

    // LLAMAR SERVIDOR: GET_NOMBRES_PUEBLOS
    char* recv = nullptr;
    char coman[] = "GET_NOMBRES_PUEBLO";
    enviarYRecibir(s, coman, &recv);

    char** nombresPueblos = nullptr;
    int cantidad = 0;

    if (recv != nullptr) {
        char* token = strtok(recv, "%L%");
        while (token != nullptr) {
            // Reservar nuevo array con espacio adicional
            char** aux = new char*[cantidad + 1];
            
            // Copiar punteros anteriores
            for (int i = 0; i < cantidad; i++) {
                aux[i] = nombresPueblos[i];
            }

            // Agregar nuevo token
            aux[cantidad] = new char[strlen(token) + 1];
            strcpy(aux[cantidad], token);
            cantidad++;

            // Liberar array anterior
            delete nombresPueblos;
            nombresPueblos = aux;

            // Obtener siguiente token
            token = strtok(nullptr, "%L%");
        }
    }

    delete[] recv;

    SeleccionPantalla* seleccionPueblo = new SeleccionPantalla("pueblo", nombresPueblos, cantidad);
    connect(seleccionPueblo, &SeleccionPantalla::seleccionada, this, [=](int indicePueblo) {

        //LLAMAR SERVIDOR: GET_PUEBLO
        char* recv = nullptr;
        char* comando = new char[11+strlen(nombresPueblos[indicePueblo])+1];
        strcpy(comando, "GET_PUEBLO|");
        strcat(comando, nombresPueblos[indicePueblo]);
        enviarYRecibir(s, comando, &recv);

        Pueblo* pueblo = new Pueblo(recv);
        seleccionPueblo->close();

        delete[] recv;
        delete[] comando;

        // Pantalla selección de zona
        int cantidadZonas = pueblo->getCantZonas();
        char** nombresZonas = new char*[cantidadZonas];
        for (int i = 0; i < cantidadZonas; ++i) {
            const char* nombreZona = pueblo->getZonas()[i]->getNombre();
            nombresZonas[i] = new char[strlen(nombreZona) + 1];
            strcpy(nombresZonas[i], nombreZona);
        }

        SeleccionPantalla* seleccionZona = new SeleccionPantalla("zona", nombresZonas, cantidadZonas);
        connect(seleccionZona, &SeleccionPantalla::seleccionada, this, [=](int indiceZona) {

            // Pantalla selección de personaje
            Zona* zonaSeleccionada = pueblo->getZonas()[indiceZona];
            int cantidadPersonajes = zonaSeleccionada->getCantP();
            char** nombresPersonajes = new char*[cantidadPersonajes];
            for (int i = 0; i < cantidadPersonajes; ++i) {
                nombresPersonajes[i] = new char[strlen(zonaSeleccionada->getPersonajes()[i]->getNombre()) + 1];
                strcpy(nombresPersonajes[i], zonaSeleccionada->getPersonajes()[i]->getNombre());
            }

            SeleccionPantalla* seleccionPersonaje = new SeleccionPantalla("personaje", nombresPersonajes, cantidadPersonajes);
            connect(seleccionPersonaje, &SeleccionPantalla::seleccionada, this, [=](int indicePersonaje) {
                Personaje* personajeSeleccionado = zonaSeleccionada->getPersonajes()[indicePersonaje];
                str1 = new char[strlen(personajeSeleccionado->convertirAChar())+1];
                strcpy(str1, personajeSeleccionado->convertirAChar());
                seleccionPersonaje->close();

                // Crear la ventana de edicion de personaje
                PantallaAnadirPersonaje* pantallaPersonaje = new PantallaAnadirPersonaje(personajeSeleccionado);
                connect(pantallaPersonaje, &::PantallaAnadirPersonaje::personajeGuardado, this, &PantallaModificar::modificarPersonaje);
                pantallaPersonaje->show();

                this->hide();

                // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
                connect(pantallaPersonaje, &PantallaAnadirPersonaje::volverAlMenu, this, [=]() {
                    this->hide();              // Aseguramos reinicio del estado
                    this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
                    this->showFullScreen();    // Forzamos pantalla completa
                });


            });

            seleccionPersonaje->show();
            seleccionZona->close();
        });

        seleccionZona->show();
        seleccionPueblo->close();
    });
    seleccionPueblo->show();

}

void PantallaModificar::modificarPersonaje(const char* nombre, const char* dialogo,
                                      const char* pista, const char* pregunta,
                                      const char* respuesta) {

    Acertijo* acertijo = new Acertijo(pregunta, respuesta);
    Personaje* personaje = new Personaje(nombre, dialogo, pista, acertijo);
    str2 = new char[strlen(personaje->convertirAChar())];
    strcpy(str2, personaje->convertirAChar());

    //LLAMAR SERVIDOR: MOD_PERSONAJE
    char* recv = nullptr;
    char* comando = new char[11+strlen(str2)+1+strlen(str1)+1];
    strcpy(comando, "MOD_PERSONAJE|");
    strcat(comando, str2);
    strcat(comando, "|");
    strcat(comando, str1);
    enviarYRecibir(s, comando, &recv);

    delete[] recv;

    this->hide();              // Aseguramos reinicio del estado
    this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
    this->showFullScreen();    // Forzamos pantalla completa
}

void PantallaModificar::modificarZona(const char* nombre, const char* descripcion, Personaje** personajes,
                                 const int cantP) {

    Zona* zona = new Zona(nombre, descripcion, personajes, cantP);
    str2 = new char[strlen(zona->convertirAChar())];
    strcpy(str2, zona->convertirAChar());

    //LLAMAR SERVIDOR: MOD_ZONA
    char* recv = nullptr;
    char* comando = new char[11+strlen(str2)+1+strlen(str1)+1];
    strcpy(comando, "MOD_ZONA|");
    strcat(comando, str2);
    strcat(comando, "|");
    strcat(comando, str1);
    enviarYRecibir(s, comando, &recv);

    this->hide();              // Aseguramos reinicio del estado
    this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
    this->showFullScreen();    // Forzamos pantalla completa
}

void PantallaModificar::modificarPueblo(const char* nombre, const char* introduccion, const char* lugarMisterio,
                                   const char* misterio, Zona** zonas, const int cantZonas) {

    Pueblo* pueblo = new Pueblo(nombre, introduccion, lugarMisterio, misterio, zonas, cantZonas);
    str2 = new char[strlen(pueblo->convertirAChar())];
    strcpy(str2, pueblo->convertirAChar());

    //LLAMAR SERVIDOR: MOD_PUEBLO
    char* recv = nullptr;
    char* comando = new char[11+strlen(str2)+1+strlen(str1)+1];
    strcpy(comando, "MOD_PUEBLO|");
    strcat(comando, str2);
    strcat(comando, "|");
    strcat(comando, str1);
    enviarYRecibir(s, comando, &recv);

    this->hide();              // Aseguramos reinicio del estado
    this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
    this->showFullScreen();    // Forzamos pantalla completa
}

PantallaModificar::~PantallaModificar(){
    if (str1) {
        delete[] str1;
    }

    if (str2) {
        delete[] str2;
    }
}