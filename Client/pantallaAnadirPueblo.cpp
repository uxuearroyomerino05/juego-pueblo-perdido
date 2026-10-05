#include "PantallaAnadirPueblo.h"
#include "PantallaAnadirZona.h"
#include <Qscrollarea>
#include <QApplication>
#include <QScreen>
#include <QMessageBox>

PantallaAnadirPueblo::PantallaAnadirPueblo(Pueblo* pueblo, QWidget* parent)
    : QWidget(parent), indiceZonaAsignado(-1) {

    this->puebloActual = pueblo;

    // Escalado relativo
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();
    int fontSize = screenHeight / 35;
    int labelFontSize = screenHeight / 40;
    int inputHeight = screenHeight * 0.3;
    int buttonHeight = screenHeight / 12;

    // Estilo de fuente común
    QFont fuente("Verdana", fontSize);
    QFont fuenteLabel("Verdana", labelFontSize);

    //Pantalla
    QVBoxLayout* layout = new QVBoxLayout(this);

    inputNombrePueblo = new QLineEdit(this);
    inputNombrePueblo->setPlaceholderText("Nombre del pueblo");
    inputNombrePueblo->setFont(fuenteLabel);

    inputIntroduccion = new QTextEdit(this);
    inputIntroduccion->setPlaceholderText("Introducción del pueblo");
    inputIntroduccion->setFont(fuenteLabel);
    inputIntroduccion->setMinimumHeight(inputHeight);

    inputLugarMisterio = new QLineEdit(this);
    inputLugarMisterio->setPlaceholderText("Lugar del misterio");
    inputLugarMisterio->setFont(fuenteLabel);

    inputMisterio = new QTextEdit(this);
    inputMisterio->setPlaceholderText("Descripción del misterio");
    inputMisterio->setFont(fuenteLabel);
    inputMisterio->setMinimumHeight(inputHeight);

    lCantZ = new QLabel("Cantidad de zonas:", this);
    lCantZ->setFont(fuenteLabel);

    spinCantidadZonas = new QSpinBox(this);
    spinCantidadZonas->setFont(fuenteLabel);
    spinCantidadZonas->setMinimumHeight(buttonHeight);
    spinCantidadZonas->setMinimum(1);
    spinCantidadZonas->setMaximum(10);
    connect(spinCantidadZonas, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &PantallaAnadirPueblo::actualizarListaZonas);

    layoutZonas = new QVBoxLayout();

    btnGuardar = new QPushButton("Guardar", this);
    btnGuardar->setFont(fuente);
    btnGuardar->setMinimumHeight(buttonHeight);
    connect(btnGuardar, &QPushButton::clicked, this, &PantallaAnadirPueblo::guardarPueblo);

    btnCancelar = new QPushButton("Cancelar", this);
    btnCancelar->setFont(fuente);
    btnCancelar->setMinimumHeight(buttonHeight);
    connect(btnCancelar, &QPushButton::clicked, this, &PantallaAnadirPueblo::cancelar);

    // Layout de botones
    QHBoxLayout* layoutBotones = new QHBoxLayout();
    layoutBotones->addWidget(btnCancelar);
    layoutBotones->addWidget(btnGuardar);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Crear widget contenedor para el scroll
    QWidget* widgetScroll = new QWidget();
    widgetScroll->setLayout(layoutZonas);
    scrollArea->setWidget(widgetScroll);

    layout->addWidget(inputNombrePueblo);
    layout->addWidget(inputIntroduccion);
    layout->addWidget(inputLugarMisterio);
    layout->addWidget(inputMisterio);
    layout->addWidget(lCantZ);
    layout->addWidget(spinCantidadZonas);
    layout->addWidget(scrollArea);
    layout->addLayout(layoutBotones);

    setLayout(layout);
    setWindowTitle("Añadir nuevo pueblo");

    if (puebloActual) {
        inputNombrePueblo->setText(puebloActual->getNombre());
        inputIntroduccion->setPlainText(puebloActual->getIntroduccion());
        inputLugarMisterio->setText(puebloActual->getLugarMisterio());
        inputMisterio->setPlainText(puebloActual->getMisterio());

        int cantidadZonas = puebloActual->getCantZonas();
        spinCantidadZonas->setValue(cantidadZonas);

        zonas.resize(cantidadZonas);
        for (int i = 0; i < cantidadZonas; ++i) {
            zonas[i] = puebloActual->getZonas()[i];
        }

        actualizarListaZonas(cantidadZonas);
    } else {
        actualizarListaZonas(1);
    }


    showFullScreen();
}

PantallaAnadirPueblo::~PantallaAnadirPueblo() {}

void PantallaAnadirPueblo::actualizarListaZonas(int cantidad) {
    // Escalado relativo
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();
    int fontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;
    QFont fuente("Verdana", fontSize);

    // Limpiar anteriores
    QLayoutItem* item;
    while ((item = layoutZonas->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    botonesZona.clear();
    zonas.resize(cantidad, nullptr);

    for (int i = 0; i < cantidad; ++i) {
        QPushButton* boton = new QPushButton(this);
        boton->setText(zonas[i] ? zonas[i]->getNombre() : QString("Añadir zona %1").arg(i + 1));
        boton->setFont(fuente);
        btnGuardar->setMinimumHeight(buttonHeight);
        layoutZonas->addWidget(boton);
        botonesZona.push_back(boton);

        connect(boton, &QPushButton::clicked, this, [this, i]() {
            editarZona(i);
        });
    }
}

void PantallaAnadirPueblo::editarZona(int indice) {
    PantallaAnadirZona* pantalla;

    if(zonas[indice]){
        pantalla= new PantallaAnadirZona(zonas[indice]);
    } else {
        pantalla= new PantallaAnadirZona();
    }

    pantalla->setAttribute(Qt::WA_DeleteOnClose);

    // Conectar la señal que recibirá los datos de la zona y actualizar la lista
    connect(pantalla, &PantallaAnadirZona::zonaGuardado, this, [=](const char* nombre, const char* descripcion, Personaje** personajes, const int cantP) {
        zonas[indice] = new Zona(nombre, descripcion, personajes, cantP);
        botonesZona[indice]->setText(zonas[indice]->getNombre());
    });

    pantalla->show();
    pantalla->raise();
}

void PantallaAnadirPueblo::guardarPueblo() {
    QString nombre = inputNombrePueblo->text();
    QString introduccion = inputIntroduccion->toPlainText();
    QString lugarMisterio = inputLugarMisterio->text();
    QString misterio = inputMisterio->toPlainText();
    int cantZonas = zonas.size();

    if (nombre.isEmpty() || introduccion.isEmpty() || lugarMisterio.isEmpty() || misterio.isEmpty()) {
        QMessageBox::warning(this, "Faltan campos", "Rellena todos los campos obligatorios.");
        return;
    }

    if (spinCantidadZonas->value() != cantZonas) {
        QMessageBox::warning(this, "Zonas incompletas", "¡La cantidad de zonas elegida y los datos de las zonas no coinciden!");
        return;
    }

    // Crear pueblo
    int cantidad = zonas.size();
    Zona** arregloZonas = zonas.empty() ? nullptr : &zonas[0];

    emit puebloGuardado(nombre.toStdString().c_str(),
                        introduccion.toStdString().c_str(),
                        lugarMisterio.toStdString().c_str(),
                        misterio.toStdString().c_str(),
                        arregloZonas,
                        cantidad);

    close();
}

void PantallaAnadirPueblo::cancelar() {
    emit volverAlMenu();
    close();
}
