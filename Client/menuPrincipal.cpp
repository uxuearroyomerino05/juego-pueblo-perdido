#include "menuPrincipal.h"
#include "PantallaResolverMisterio.h"
#include "seleccionPantalla.h"
#include "menuInicio.h"
#include "menuInteraccionPersonaje.h"

#include <QMessageBox>
#include <QTimer>
#include <QFont>
#include <QScreen>
#include <QStyle>
#include <QApplication>
#include <winsock2.h>

MenuPrincipal::MenuPrincipal(Jugador* jugador, Pueblo* puebloSeleccionado, const SOCKET s, QWidget *parent)
    : QWidget(parent), jugador(jugador), pueblo(puebloSeleccionado)
{
    this->s = s;

    // Pantalla completa
    showFullScreen();

    // Obtener dimensiones para escalado
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();

    // Tamaños relativos (ajustables)
    int titleFontSize = screenHeight / 20;
    int introFontSize = screenHeight / 40;
    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;

    // Layout principal
    layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(50, 50, 50, 50);

    // Estilo oscuro general
    this->setStyleSheet("background-color: #121212; color: #f0f0f0;");

    // Título
    titulo = new QLabel("Bienvenido a " + QString(pueblo->getNombre()), this);
    titulo->setAlignment(Qt::AlignCenter);
    QFont fontTitulo("Georgia", titleFontSize, QFont::Bold);
    titulo->setFont(fontTitulo);
    titulo->setStyleSheet("color: #ffffff;");

    // Texto de introducción
    introduccionLabel = new QLabel(this);
    introduccionLabel->setWordWrap(true);
    QFont fontIntro("Times New Roman", introFontSize);
    introduccionLabel->setFont(fontIntro);
    introduccionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    introduccionLabel->setStyleSheet("color: #cccccc;");

    // Botón para saltar introducción
    btnSaltarIntro = new QPushButton("Saltar introducción", this);
    btnSaltarIntro->setMinimumHeight(buttonHeight);
    btnSaltarIntro->setFont(QFont("Verdana", buttonFontSize));
    btnSaltarIntro->setStyleSheet(
        "QPushButton {"
        "  background-color: #333333;"
        "  color: white;"
        "  border: 2px solid #555555;"
        "  border-radius: 10px;"
        "  padding: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #444444;"
        "  border-color: #777777;"
        "}"
        );

    connect(btnSaltarIntro, &QPushButton::clicked, this, &MenuPrincipal::mostrarMenu);

    // Añadir al layout
    layout->addWidget(titulo);
    layout->addWidget(introduccionLabel);
    layout->addWidget(btnSaltarIntro);

    // Mostrar texto gradualmente
    mostrarTextoGradualmente(introduccionLabel, QString(pueblo->getIntroduccion()));
}
void MenuPrincipal::mostrarTextoGradualmente(QLabel* label, const QString& texto)
{
    label->clear();
    static int index = 0;
    index = 0;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (index < texto.length()) {
            label->setText(label->text() + texto[index]);
            index++;
        } else {
            timer->stop();
            // Ya no mostramos el menú automáticamente
        }
    });

    timer->start(50);  // ← Más lento (antes 30ms, ahora 50ms por carácter)
}

void MenuPrincipal::mostrarMenu()
{
    if (timer && timer->isActive()) {
        timer->stop();
    }

    // Limpiar introducción
    layout->removeWidget(titulo);
    layout->removeWidget(introduccionLabel);
    layout->removeWidget(btnSaltarIntro);
    titulo->deleteLater();
    introduccionLabel->deleteLater();
    btnSaltarIntro->deleteLater();

    // Obtener tamaño pantalla
    int screenHeight = QApplication::primaryScreen()->size().height();
    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;

    // Crear y configurar botones
    auto crearBoton = [&](const QString& texto) {
        QPushButton* boton = new QPushButton(texto, this);
        boton->setMinimumHeight(buttonHeight);
        boton->setFont(QFont("Verdana", buttonFontSize));
        boton->setStyleSheet(
            "QPushButton {"
            "  background-color: #1f1f1f;"
            "  color: #dddddd;"
            "  border: 2px solid #3c3c3c;"
            "  border-radius: 10px;"
            "  padding: 12px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #2a2a2a;"
            "  border-color: #5c5c5c;"
            "}"
            );
        layout->addWidget(boton);
        return boton;
    };

    btnInvestigar = crearBoton("🔍 Investigar zona");
    btnVerObjetos = crearBoton("🎒 Ver objetos");
    btnResolverMisterio = crearBoton("🕵️ Resolver misterio");
    btnVolver = crearBoton("⬅️ Volver al menú");

    // Conectar señales
    connect(btnInvestigar, &QPushButton::clicked, this, &MenuPrincipal::investigarZona);
    connect(btnVerObjetos, &QPushButton::clicked, this, &MenuPrincipal::verObjetos);
    connect(btnResolverMisterio, &QPushButton::clicked, this, &MenuPrincipal::resolverMisterio);
    connect(btnVolver, &QPushButton::clicked, this, &MenuPrincipal::volverAlMenu);
}

void MenuPrincipal::investigarZona() {
    int cantidadZonas = pueblo->getCantZonas();
    char** nombresZonas = new char*[cantidadZonas];
    for (int i = 0; i < cantidadZonas; ++i) {
        nombresZonas[i] = pueblo->getZonas()[i]->getNombre();
    }

    // Pantalla selección de zona
    SeleccionPantalla* seleccionZona = new SeleccionPantalla("zona", nombresZonas, cantidadZonas);
    connect(seleccionZona, &SeleccionPantalla::seleccionada, this, [=](int indiceZona) {
        // Obtener los personajes
        Zona* zonaSeleccionada = pueblo->getZonas()[indiceZona];
        int cantidadPersonajes = zonaSeleccionada->getCantP();
        char** nombresPersonajes = new char*[cantidadPersonajes];
        for (int i = 0; i < cantidadPersonajes; ++i) {
            nombresPersonajes[i] = zonaSeleccionada->getPersonajes()[i]->getNombre();
        }

        SeleccionPantalla* seleccionPersonaje = new SeleccionPantalla("personaje", nombresPersonajes, cantidadPersonajes);
        connect(seleccionPersonaje, &SeleccionPantalla::seleccionada, this, [=](int indicePersonaje) {
            Personaje* personajeSeleccionado = zonaSeleccionada->getPersonajes()[indicePersonaje];
            seleccionPersonaje->close();

            //Esconder pantalla actual
            this->hide();

            // Crear la ventana de interacción con el personaje
            personajeSeleccionado->imprimirPersonaje();
            MenuInteraccionPersonaje* interaccion = new MenuInteraccionPersonaje(nullptr, personajeSeleccionado);
            interaccion->show();
            interaccion->raise();

            // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
            connect(interaccion, &MenuInteraccionPersonaje::interaccionFinalizada, this, [=]() {
                this->hide();              // Aseguramos reinicio del estado
                this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
                this->showFullScreen();    // Forzamos pantalla completa
            });

        });

        seleccionPersonaje->show();
        seleccionZona->close();
    });

    seleccionZona->show();
}

void MenuPrincipal::verObjetos() {
    QString info = "Objetos que tienes:\n";

    int cantidad = jugador->getCantObjetos();  // Asegúrate de que exista este método
    char** objetos = jugador->getObjetos();        // Devuelve char**

    for (int i = 0; i < cantidad; ++i) {
        info += "- " + QString(objetos[i]) + "\n";
    }

    QMessageBox::information(this, "Objetos", info);
}

void MenuPrincipal::resolverMisterio() {
    int cantidadZonas = pueblo->getCantZonas();
    if (jugador->getCantObjetos() < cantidadZonas) {
        QMessageBox::warning(this, "Faltan objetos", "¡Aún no tienes todos los objetos necesarios!");
        return;
    }

    // Esconder la pantalla actual
    this->hide();

    //Pantalla misterio
    PantallaResolverMisterio* misterio = new PantallaResolverMisterio(nullptr, jugador, pueblo);
    connect(misterio, &QWidget::destroyed, this, &MenuPrincipal::show);
    misterio->show();

    // Cuando se cierre la pantalla de interacción, volver a mostrar el menú principal
    connect(misterio, &PantallaResolverMisterio::volverAlMenu, this, [=]() {
        this->hide();              // Aseguramos reinicio del estado
        this->setWindowState(Qt::WindowNoState);  // Quita estados previos como minimized/maximized
        this->showFullScreen();    // Forzamos pantalla completa
    });

}

void MenuPrincipal::volverAlMenu() {
    this->close();
    MenuInicio* inicio = new MenuInicio(s); // Ajustar si se usan más pueblos
    inicio->show();
}

MenuPrincipal::~MenuPrincipal(){
    
}