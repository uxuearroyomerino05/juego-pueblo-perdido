#include "seleccionPantalla.h"
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScreen>
#include <QFont>

#include <iostream>

SeleccionPantalla::SeleccionPantalla(const char* tipo, char** nombres, int cantidad, QWidget *parent)
    : QWidget(parent)
{
    
    // Tamaño proporcional al 50% de la pantalla
    QScreen* screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();
    int width = screenSize.width() * 0.5;
    int height = screenSize.height() * 0.5;
    setFixedSize(width, height);
    move((screenSize.width() - width) / 2, (screenSize.height() - height) / 2);

    // Fuentes proporcionales
    int fontTitleSize = height / 18;
    QFont fontTitulo("Georgia", fontTitleSize);

    //Configuracion ventana
    layout = new QVBoxLayout(this);

    // Crear etiqueta
    etiqueta = new QLabel(QString("Elige el %1 a visitar:").arg(tipo), this);
    etiqueta->setFont(fontTitulo);
    layout->addWidget(etiqueta);

    // Crear scrollArea
    scrollArea = new QScrollArea(this);
    QWidget* scrollContent = new QWidget();        // Contenedor de los botones
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent); // Layout para el scroll

    // Crear botones para cada elemento en el array de nombres
    crearBotones(nombres, cantidad, scrollLayout);

    // Alinear los botones dentro del layout
    scrollLayout->setAlignment(Qt::AlignCenter);

    // Establecer el contenido del scroll
    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);  // Hacer que el área se ajuste al tamaño del contenido

    // Agregar el QScrollArea al layout principal
    layout->addWidget(scrollArea);
}

SeleccionPantalla::~SeleccionPantalla()
{
}

void SeleccionPantalla::crearBotones(char** nombres, int cantidad, QVBoxLayout* scrollLayout)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->size();
    int height = screenSize.height() * 0.5;
    int fontButtonSize = height / 28;
    QFont fontBoton("Arial", fontButtonSize);

    for (int i = 0; i < cantidad; ++i) {
        QPushButton* boton = new QPushButton(QString(nombres[i]), this);
        boton->setFont(fontBoton);
        // Conectar la señal de clic del botón a la señal de selección
        connect(boton, &QPushButton::clicked, this, [=]() {
            emit seleccionada(i);  // Emitir el índice de la selección
        });
        scrollLayout->addWidget(boton);  // Agregar los botones al layout de scroll
    }
}

