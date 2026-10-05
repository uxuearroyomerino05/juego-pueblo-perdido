#include "PantallaResolverMisterio.h"
#include "qapplication.h"
#include "qscreen.h"
#include <QDebug>
#include <QScrollArea>

PantallaResolverMisterio::PantallaResolverMisterio(QWidget* parent, Jugador* jugador, Pueblo* pueblo)
    : QWidget(parent), jugador(jugador), pueblo(pueblo), pasoActual(0) {

    // Validación
    if (jugador->getCantObjetos() < pueblo->getCantZonas()) {
        QLabel* error = new QLabel("Aún no tienes todos los objetos necesarios para resolver el misterio.", this);
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(error);
        setLayout(layout);
        return;
    }

    //Iniciar Timer
    timerEscritura = new QTimer(this);
    connect(timerEscritura, &QTimer::timeout, this, [=]() {
        if (indexTexto < textoEnProceso.length()) {
            labelTexto->setText(labelTexto->text() + textoEnProceso[indexTexto]);
            indexTexto++;
        } else {
            timerEscritura->stop();
            QTimer::singleShot(1000, this, &PantallaResolverMisterio::colocarSiguienteObjeto);
        }
    });

    // Obtener dimensiones para escalado
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();

    // Tamaños relativos (ajustables)
    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;
    int textoFontSize = screenHeight / 30;
    int textoVisibleAltura = screenHeight * 0.6;

    // QLabel para el texto
    labelTexto = new QLabel(pueblo->getLugarMisterio(), this);
    labelTexto->setWordWrap(true);
    labelTexto->setAlignment(Qt::AlignHCenter);  // Alineado arriba y centrado
    labelTexto->setFont(QFont("Verdana", textoFontSize));

    // ScrollArea con altura fija
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setFixedHeight(textoVisibleAltura);
    scrollArea->setWidget(labelTexto);

    // Botón acción
    btnAccion = new QPushButton("Colocar objetos", this);
    connect(btnAccion, &QPushButton::clicked, this, &PantallaResolverMisterio::comenzarColocacion);
    btnAccion->setMinimumHeight(buttonHeight);
    btnAccion->setFont(QFont("Verdana", buttonFontSize));

    // Botón volver
    btnVolver = new QPushButton("Volver al menú principal", this);
    btnVolver->hide();
    connect(btnVolver, &QPushButton::clicked, this, [=]() {
        close();
        emit volverAlMenu();
    });
    btnVolver->setMinimumHeight(buttonHeight);
    btnVolver->setFont(QFont("Verdana", buttonFontSize));

    // Layout principal
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);
    layout->addWidget(btnAccion);
    layout->addWidget(btnVolver);
    setLayout(layout);

    // Inicializar objetos a colocar
    for (int i = 0; i < jugador->getCantObjetos(); ++i) {
        objetosAColocar << jugador->getObjeto(i);
    }

    timerEscritura = new QTimer(this);
    connect(timerEscritura, &QTimer::timeout, this, [=]() {
        if (indexTexto < textoEnProceso.length()) {
            labelTexto->setText(labelTexto->text() + textoEnProceso[indexTexto]);
            indexTexto++;
        } else {
            timerEscritura->stop();

            // Espera un segundo antes de continuar
            QTimer::singleShot(1000, this, &PantallaResolverMisterio::colocarSiguienteObjeto);
        }
    });

    showFullScreen();
}

void PantallaResolverMisterio::comenzarColocacion() {
    btnAccion->hide();
    labelTexto->setText("Colocando objetos...");
    pasoActual = 0;
    QTimer::singleShot(1000, this, &PantallaResolverMisterio::colocarSiguienteObjeto);
}

void PantallaResolverMisterio::colocarSiguienteObjeto() {
    if (pasoActual < objetosAColocar.size()) {
        textoEnProceso = QString("\nColocas el objeto: %1\n...").arg(objetosAColocar[pasoActual]);
        pasoActual++;

        labelTexto->clear();
        indexTexto = 0;
        timerEscritura->start(50);
    } else if (pasoActual == objetosAColocar.size()) {
        // Paso final: texto adicional después del último objeto
        pasoActual++; // avanzar para no repetir esto

        textoEnProceso = "Cuando el último objeto encaja en su lugar, las inscripciones comienzan a brillar...\n"
                         "Un mecanismo oculto se activa y un compartimento secreto se abre dentro del pedestal.\n"
                         "Dentro encuentras un antiguo pergamino envuelto en seda.\n";

        labelTexto->clear();
        indexTexto = 0;
        timerEscritura->start(50);
    } else if (pasoActual == objetosAColocar.size() + 1) {
        // Mostrar el misterio final
        pasoActual++;  // para que no vuelva a entrar

        textoEnProceso = pueblo->getMisterio();
        labelTexto->clear();
        indexTexto = 0;
        timerEscritura->start(50);
    } else {
        btnVolver->show();
    }
}

void PantallaResolverMisterio::mostrarResolucion() {
    // Ahora mostrar el misterio de forma gradual
    textoEnProceso = pueblo->getMisterio(); // El misterio final
    indexTexto = 0;
    labelTexto->clear();  // Limpiar antes de empezar el misterio
    timerEscritura->start(50);  // Escribir gradualmente el misterio
}

PantallaResolverMisterio::~PantallaResolverMisterio(){

}