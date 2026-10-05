#include "PantallaAnadirZona.h"
#include "PantallaAnadirPersonaje.h"
#include <Qapplication>
#include <Qscreen>
#include "seleccionPantalla.h"
#include <QMessageBox>
#include "zona.h"

using namespace Juego;

PantallaAnadirZona::PantallaAnadirZona(Zona* zona, QWidget* parent)
    : QWidget(parent), indicePersonajeAsignado(-1) {

    this->zonaActual = zona;

    // Escalado relativo
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();
    int fontSize = screenHeight / 35;
    int labelFontSize = screenHeight / 40;
    int inputHeight = screenHeight*0.3;
    int buttonHeight = screenHeight / 12;

    // Estilo de fuente común
    QFont fuente("Verdana", fontSize);
    QFont fuenteLabel("Verdana", labelFontSize);

    //Pantalla
    QVBoxLayout* layout = new QVBoxLayout(this);

    inputNombreZona = new QLineEdit(this);
    inputNombreZona->setPlaceholderText("Nombre de la zona");
    inputNombreZona->setFont(fuenteLabel);

    inputDescripcionZona = new QTextEdit(this);
    inputDescripcionZona->setPlaceholderText("Descripción de la zona");
    inputDescripcionZona->setFont(fuenteLabel);
    inputDescripcionZona->setMinimumHeight(inputHeight);

    lCantP = new QLabel("Cantidad de personajes:", this);
    lCantP->setFont(fuenteLabel);

    spinCantidadPersonajes = new QSpinBox(this);
    spinCantidadPersonajes->setFont(fuenteLabel);
    spinCantidadPersonajes->setMinimumHeight(buttonHeight);
    spinCantidadPersonajes->setMinimum(1);
    spinCantidadPersonajes->setMaximum(10);
    connect(spinCantidadPersonajes, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &PantallaAnadirZona::actualizarListaPersonajes);

    layoutPersonajes = new QVBoxLayout();

    inputNombreObjeto = new QLineEdit(this);
    inputNombreObjeto->setPlaceholderText("Nombre del objeto asociado");
    inputNombreObjeto->setFont(fuenteLabel);

    QPushButton* botonAsignar = new QPushButton("Asignar personaje al objeto", this);
    botonAsignar->setFont(fuente);
    botonAsignar->setMinimumHeight(buttonHeight);
    labelPersonajeAsignado = new QLabel("Personaje no asignado", this);
    labelPersonajeAsignado->setFont(fuenteLabel);
    connect(botonAsignar, &QPushButton::clicked, this, &PantallaAnadirZona::asignarObjetoAPersonaje);

    btnGuardar = new QPushButton("Guardar", this);
    btnGuardar->setFont(fuente);
    btnGuardar->setMinimumHeight(buttonHeight);
    connect(btnGuardar, &QPushButton::clicked, this, &PantallaAnadirZona::guardarZona);

    btnCancelar = new QPushButton("Cancelar", this);
    btnCancelar->setFont(fuente);
    btnCancelar->setMinimumHeight(buttonHeight);
    connect(btnCancelar, &QPushButton::clicked, this, &PantallaAnadirZona::cancelar);

    //Layout
    QHBoxLayout* layoutBotones = new QHBoxLayout();
    layoutBotones->addWidget(btnCancelar);
    layoutBotones->addWidget(btnGuardar);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Crear widget contenedor para el scroll
    QWidget* widgetScroll = new QWidget();
    widgetScroll->setLayout(layoutPersonajes);
    scrollArea->setWidget(widgetScroll);


    layout->addWidget(inputNombreZona);
    layout->addWidget(inputDescripcionZona);
    layout->addWidget(lCantP);
    layout->addWidget(spinCantidadPersonajes);
    layout->addWidget(scrollArea);
    layout->addWidget(inputNombreObjeto);
    layout->addWidget(botonAsignar);
    layout->addWidget(labelPersonajeAsignado);
    layout->addLayout(layoutBotones);

    setLayout(layout);
    setWindowTitle("Añadir nueva zona");

    if (zonaActual != nullptr) {
        inputNombreZona->setText(zonaActual->getNombre());
        inputDescripcionZona->setText(zonaActual->getDescripcion());
        Personaje** arreglo = zonaActual->getPersonajes();
        personajes = std::vector<Personaje*>(arreglo, arreglo + zonaActual->getCantP());

        //Encontrar objeto y personaje asigando
        for(int i=0; i<zonaActual->getCantP(); i++){
            if(zonaActual->getPersonajes()[i]->getObjeto() != nullptr){
                this->indicePersonajeAsignado = i;
                inputNombreObjeto->setText(zonaActual->getPersonajes()[i]->getObjeto());
                labelPersonajeAsignado->setText(QString("Asignado a: %1").arg(personajes[i]->getNombre()));
            }
        }
        spinCantidadPersonajes->setValue(zonaActual->getCantP());
        actualizarListaPersonajes(zonaActual->getCantP());
    } else {
        actualizarListaPersonajes(1);
    }

    showFullScreen();

}

PantallaAnadirZona::~PantallaAnadirZona() {}

void PantallaAnadirZona::actualizarListaPersonajes(int cantidad) {
    // Escalado relativo
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();
    int fontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;
    QFont fuente("Verdana", fontSize);

    // Limpiar anteriores
    QLayoutItem* item;
    while ((item = layoutPersonajes->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    botonesPersonaje.clear();
    personajes.resize(cantidad, nullptr);  // Redimensionamos el vector de personajes

    for (int i = 0; i < cantidad; ++i) {
        QPushButton* boton = new QPushButton(this);
        boton->setText(personajes[i] ? personajes[i]->getNombre() : QString("Añadir personaje %1").arg(i + 1));
        boton->setFont(fuente);
        btnGuardar->setMinimumHeight(buttonHeight);
        layoutPersonajes->addWidget(boton);
        botonesPersonaje.push_back(boton);

        connect(boton, &QPushButton::clicked, this, [this, i]() {
            editarPersonaje(i);
        });
    }
}

void PantallaAnadirZona::editarPersonaje(int indice) {

    PantallaAnadirPersonaje* pantalla;
    if(personajes[indice]){
        pantalla= new PantallaAnadirPersonaje(personajes[indice]);
    } else {
        pantalla= new PantallaAnadirPersonaje();
    }

    pantalla->setAttribute(Qt::WA_DeleteOnClose);

    // Conectar la señal que recibirá los datos del personaje y actualizar la lista
    connect(pantalla, &PantallaAnadirPersonaje::personajeGuardado, this, [=](const char* nombre, const char* dialogo, const char* pista, const char* pregunta, const char* respuesta) {
        personajes[indice] = new Personaje(nombre, dialogo, pista, new Acertijo(pregunta, respuesta));
        botonesPersonaje[indice]->setText(personajes[indice]->getNombre());
    });

    pantalla->show();
    pantalla->raise();
}

void PantallaAnadirZona::asignarObjetoAPersonaje() {

    int cantidad = personajes.size();

    if(cantidad != spinCantidadPersonajes->value()){
        QMessageBox::warning(this, "Faltan personajes", "¡La cantidad de personajes elegida y los datos de los personajes que se tiene no es la misma!");
        return;
    }

    char** nombres = new char*[personajes.size()];
    char noDefinido[] = "[No definido]";

    for (int i=0; i<cantidad; i++) {
        nombres[i] = (personajes[i] ? personajes[i]->getNombre() : noDefinido);
    }

    SeleccionPantalla* selector = new SeleccionPantalla("Personaje", nombres, cantidad);
    connect(selector, &SeleccionPantalla::seleccionada, this, [=](int indice) {
        if (indice >= 0 && personajes[indice]) {
            indicePersonajeAsignado = indice;
            labelPersonajeAsignado->setText(QString("Asignado a: %1").arg(personajes[indice]->getNombre()));
        }
    });
    selector->show();
}

void PantallaAnadirZona::guardarZona() {
    QString nombre = inputNombreZona->text();
    QString descripcion = inputDescripcionZona->toPlainText();
    QString nombreObjeto = inputNombreObjeto->text();

    if (nombre.isEmpty() || descripcion.isEmpty() || nombreObjeto.isEmpty()) {
        QMessageBox::warning(this, "Faltan campos", "Rellena todos los campos obligatorios.");
        return;
    }

    for (Personaje* p : personajes) {
        if (!p) {
            QMessageBox::warning(this, "Personajes incompletos", "Todos los personajes deben estar definidos.");
            return;
        }
    }

    if (indicePersonajeAsignado < 0 || !personajes[indicePersonajeAsignado]) {
        QMessageBox::warning(this, "Falta asignar", "Debes asignar un personaje al objeto.");
        return;
    }

    // Crear objeto y asignarlo
    personajes[indicePersonajeAsignado]->setObjeto(nombreObjeto.toUtf8().constData());

    // Crear zona
    int cantidad = personajes.size();
    Personaje** arregloPersonajes = personajes.empty() ? nullptr : &personajes[0];

    emit zonaGuardado(nombre.toStdString().c_str(),
                      descripcion.toStdString().c_str(),
                      arregloPersonajes,
                      cantidad);

    close();
}


void PantallaAnadirZona::cancelar() {
    emit volverAlMenu();
    close();
}
