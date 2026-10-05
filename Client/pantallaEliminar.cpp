#include "pantallaEliminar.h"
#include "qmessagebox.h"
#include "seleccionPantalla.h"
#include <QApplication>
#include <QScreen>
#include <winsock2.h>
#include "protocoloMensages.h"

PantallaEliminar::PantallaEliminar(const SOCKET s, QWidget* parent)
    : QWidget(parent) {

    this->s=s;

    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();
    int fontSize = screenHeight / 30;
    int buttonHeight = screenHeight / 12;
    QFont fuente("Verdana", fontSize);

    btnEliminarPueblo = new QPushButton("Eliminar Pueblo", this);
    btnEliminarPueblo->setFont(fuente);
    btnEliminarPueblo->setMinimumHeight(buttonHeight);

    btnEliminarZona = new QPushButton("Eliminar Zona", this);
    btnEliminarZona->setFont(fuente);
    btnEliminarZona->setMinimumHeight(buttonHeight);

    btnEliminarPersonaje = new QPushButton("Eliminar Personaje", this);
    btnEliminarPersonaje->setFont(fuente);
    btnEliminarPersonaje->setMinimumHeight(buttonHeight);

    btnVolver = new QPushButton("Volver", this);
    btnVolver->setFont(fuente);
    btnVolver->setMinimumHeight(buttonHeight);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(btnEliminarPueblo);
    layout->addWidget(btnEliminarZona);
    layout->addWidget(btnEliminarPersonaje);
    layout->addWidget(btnVolver);
    setLayout(layout);

    connect(btnEliminarPueblo, &QPushButton::clicked, this, &PantallaEliminar::eliminarPueblo);
    connect(btnEliminarZona, &QPushButton::clicked, this, &PantallaEliminar::eliminarZona);
    connect(btnEliminarPersonaje, &QPushButton::clicked, this, &PantallaEliminar::eliminarPersonaje);
    connect(btnVolver, &QPushButton::clicked, this, &PantallaEliminar::volverAlMenu);

    setWindowTitle("Eliminar elementos");
    showFullScreen();
}

PantallaEliminar::~PantallaEliminar() {}

void PantallaEliminar::volverAlMenu() {
    emit interaccionFinalizada();
    this->close();
}


void PantallaEliminar::eliminarPueblo() {
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

        Pueblo* puebloSeleccionada = new Pueblo(recv);
        seleccionPueblo->close();

        delete[] recv;
        delete[] comando;

        //Abrir mensage de confirmacion
        QString nombre = puebloSeleccionada->getNombre();
        QMessageBox::StandardButton confirmacion;
        confirmacion = QMessageBox::question(this, "Eliminar pueblo",
                                             "¿Estás seguro que querés eliminar el pueblo \"" + nombre + "\"?",
                                             QMessageBox::Yes | QMessageBox::No);

        if (confirmacion == QMessageBox::Yes) {
            //LLAMAR SERVIDOR: DEL_PUEBLO
            char* recv = nullptr;
            char* comando = new char[11+strlen(puebloSeleccionada->convertirAChar())+1];
            strcpy(comando, "DEL_PUEBLO|");
            strcat(comando, puebloSeleccionada->convertirAChar());
            enviarYRecibir(s, comando, &recv);

            delete[] recv;

        }

        seleccionPueblo->close();
    });
    seleccionPueblo->show();
}


void PantallaEliminar::eliminarZona() {
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

        Pueblo* puebloSeleccionada = new Pueblo(recv);

        delete[] recv;
        delete[] comando;

        // Pantalla selección de zona
        int cantidadZonas = puebloSeleccionada->getCantZonas();
        char** nombresZonas = new char*[cantidadZonas];
        for (int i = 0; i < cantidadZonas; ++i) {
            const char* nombreZona = puebloSeleccionada->getZonas()[i]->getNombre();
            nombresZonas[i] = new char[strlen(nombreZona) + 1];
            strcpy(nombresZonas[i], nombreZona);
        }

        SeleccionPantalla* seleccionZona = new SeleccionPantalla("zona", nombresZonas, cantidadZonas);
        connect(seleccionZona, &SeleccionPantalla::seleccionada, this, [=](int indiceZona) {

            Zona* zonaSeleccionada =  puebloSeleccionada->getZonas()[indiceZona];
            seleccionZona->close();

            //Abriri mensage de confirmacion
            QString nombre = zonaSeleccionada->getNombre();
            QMessageBox::StandardButton confirmacion;
            confirmacion = QMessageBox::question(this, "Eliminar zona",
                                                 "¿Estás seguro que querés eliminar el zona \"" + nombre + "\"?",
                                                 QMessageBox::Yes | QMessageBox::No);

            if (confirmacion == QMessageBox::Yes) {
                //LLAMAR SERVIDOR: DEL_ZONA
                char* recv = nullptr;
                char* comando = new char[11+strlen(zonaSeleccionada->convertirAChar())+1];
                strcpy(comando, "DEL_ZONA|");
                strcat(comando, zonaSeleccionada->convertirAChar());
                enviarYRecibir(s, comando, &recv);

                delete[] recv;

            }

        });

        seleccionZona->show();
        seleccionPueblo->close();
    });
    seleccionPueblo->show();
}

void PantallaEliminar::eliminarPersonaje() {

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

        Pueblo* puebloSeleccionada = new Pueblo(recv);

        delete[] recv;
        delete[] comando;

        // Pantalla selección de zona
        int cantidadZonas = puebloSeleccionada->getCantZonas();
        char** nombresZonas = new char*[cantidadZonas];
        for (int i = 0; i < cantidadZonas; ++i) {
            const char* nombreZona = puebloSeleccionada->getZonas()[i]->getNombre();
            nombresZonas[i] = new char[strlen(nombreZona) + 1];
            strcpy(nombresZonas[i], nombreZona);
        }

        SeleccionPantalla* seleccionZona = new SeleccionPantalla("zona", nombresZonas, cantidadZonas);
        connect(seleccionZona, &SeleccionPantalla::seleccionada, this, [=](int indiceZona) {

            // Pantalla selección de personaje
            Zona* zonaSeleccionada = puebloSeleccionada->getZonas()[indiceZona];

            int cantidadPersonajes = zonaSeleccionada->getCantP();
            char** nombresPersonajes = new char*[cantidadPersonajes];
            for (int i = 0; i < cantidadPersonajes; ++i) {
                const char* nombrePersonaje = zonaSeleccionada->getPersonajes()[i]->getNombre();
                nombresPersonajes[i] =new char[strlen(nombrePersonaje) + 1];
                strcpy(nombresPersonajes[i], nombrePersonaje);
            }

            SeleccionPantalla* seleccionPersonaje = new SeleccionPantalla("personaje", nombresPersonajes, cantidadPersonajes);
            connect(seleccionPersonaje, &SeleccionPantalla::seleccionada, this, [=](int indicePersonaje) {
                Personaje* personajeSeleccionado = zonaSeleccionada->getPersonajes()[indicePersonaje];
                seleccionPersonaje->close();

                //Abriri mensage de confirmacion
                QString nombre = personajeSeleccionado->getNombre();
                QMessageBox::StandardButton confirmacion;
                confirmacion = QMessageBox::question(this, "Eliminar personaje",
                                                     "¿Estás seguro que querés eliminar el personaje \"" + nombre + "\"?",
                                                     QMessageBox::Yes | QMessageBox::No);

                if (confirmacion == QMessageBox::Yes) {
                    //LLAMAR SERVIDOR: DEL_PERSONAJE
                    char* recv = nullptr;
                    char* comando = new char[11+strlen(personajeSeleccionado->convertirAChar())+1];
                    strcpy(comando, "DEL_PERSONAJE|");
                    strcat(comando, personajeSeleccionado->convertirAChar());
                    enviarYRecibir(s, comando, &recv);

                    delete[] recv;

                }

            });

            seleccionPersonaje->show();
            seleccionZona->close();
        });

        seleccionZona->show();
        seleccionPueblo->close();
    });
    seleccionPueblo->show();

}
