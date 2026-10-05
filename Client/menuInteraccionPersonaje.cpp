#include "menuInteraccionPersonaje.h"
#include "personaje.h"
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QScrollArea>

MenuInteraccionPersonaje::MenuInteraccionPersonaje(QWidget* parent, Juego::Personaje* personaje)
    : QWidget(parent), personaje(personaje), currentStep(0) {

    if (!personaje) {
        qDebug() << "ERROR: personaje es nullptr";
        return;
    }

    // Obtener dimensiones para escalado
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();

    // Tamaños relativos (ajustables)
    int introFontSize = screenHeight / 20;
    int buttonFontSize = screenHeight / 35;
    int buttonHeight = screenHeight / 12;
    int textoVisibleAltura = screenHeight * 0.6;

    // Configuración de las frases que se muestran cuando la respuesta es incorrecta
    configurarFrasesIncorrectas();

    // Configuración de la UI
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Fuentes personalizadas
    QFont textFont("Arial", introFontSize);
    QFont buttonFont("Arial", buttonFontSize);

    // Label para el personaje
    labelPersonaje = new QLabel("", this);
    labelPersonaje->setFont(textFont);
    labelPersonaje->setAlignment(Qt::AlignCenter);
    labelPersonaje->setWordWrap(true);
    labelPersonaje->setStyleSheet("padding: 10px; color: white;");


    // Label para el diálogo
    labelDialogo = new QLabel("", this);
    labelDialogo->setFont(textFont);
    labelDialogo->setAlignment(Qt::AlignCenter);
    labelDialogo->setWordWrap(true);
    labelDialogo->setStyleSheet("padding: 10px; color: white;");

    // Label para el acertijo
    labelAcertijo = new QLabel("", this);
    labelAcertijo->setFont(textFont);
    labelAcertijo->setAlignment(Qt::AlignCenter);
    labelAcertijo->setWordWrap(true);

    // Label para la pista
    labelPista = new QLabel("", this);
    labelPista->setWordWrap(true);
    labelPista->setFont(textFont);
    labelPista->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Label para el objeto
    labelObjeto = new QLabel("", this);
    labelObjeto->setFont(textFont);
    labelObjeto->setAlignment(Qt::AlignCenter);
    labelObjeto->setWordWrap(true);

    // Campo de texto para la respuesta
    inputRespuesta = new QLineEdit(this);
    inputRespuesta->setFont(textFont);

    // Botón para enviar la respuesta
    btnEnviar = new QPushButton("Enviar Respuesta", this);
    btnEnviar->setFont(buttonFont);
    btnEnviar->setFixedHeight(buttonHeight);
    btnEnviar->setEnabled(false);  // Inicialmente deshabilitado

    // Botón para avanzar el diálogo
    btnSiguiente = new QPushButton("Siguiente", this);
    btnSiguiente->setFont(buttonFont);
    btnSiguiente->setFixedHeight(buttonHeight);

    // ScrollArea con altura fija
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setFixedHeight(textoVisibleAltura);

    // Añadir widgets al layout
    layout->addWidget(scrollArea);
    layout->addWidget(inputRespuesta);
    layout->addWidget(btnEnviar);
    layout->addWidget(btnSiguiente);

    // Conectar los botones
    connect(btnEnviar, &QPushButton::clicked, this, &MenuInteraccionPersonaje::verificarRespuesta);
    connect(btnSiguiente, &QPushButton::clicked, this, &MenuInteraccionPersonaje::mostrarSiguientePantalla);

    // Establecer la lista de diálogo del personaje
    dialogoList = QString(personaje->getDialogo()).split("\n");

    setLayout(layout);
    showFullScreen();

    // Labels configuracion texto
    timerTexto = new QTimer(this);
    connect(timerTexto, &QTimer::timeout, this, &MenuInteraccionPersonaje::mostrarTextoGradualmente);
    currentStep = 0;
    mostrarSiguientePantalla();

}

void MenuInteraccionPersonaje::mostrarSiguientePantalla() {
    // Ocultar todo al principio para evitar cosas colgadas
    //scrollArea->removeWidget(labelPersonaje);
    //scrollArea->removeWidget(labelDialogo);
    //scrollArea->removeWidget(labelAcertijo);
    inputRespuesta->hide();
    btnEnviar->hide();
    //scrollArea->removeWidget(labelPista);
    //scrollArea->removeWidget(labelObjeto);

    switch (currentStep) {
    case 0:  // Mostrar saludo
        iniciarTextoGradual(labelPersonaje, "¡Hola! Soy " + QString(personaje->getNombre()));
        break;

    case 1:  // Mostrar el diálogo
        iniciarTextoGradual(labelDialogo, personaje->getDialogo());
        break;

    case 2:  // Mostrar la pregunta
        iniciarTextoGradual(labelAcertijo, personaje->getAcertijo()->getPregunta());
        inputRespuesta->show();
        btnEnviar->show();


        btnEnviar->setEnabled(true);
        btnSiguiente->setEnabled(false);
        break;

    case 3:  // Mostrar la pista
        iniciarTextoGradual(labelPista, personaje->getPista());
        break;

    case 4: {  // Mostrar el objeto (si tiene)
        char* objeto = personaje->getObjeto();
        if (objeto != nullptr) {
            iniciarTextoGradual(labelObjeto, "¡Has recibido un objeto: " + QString(objeto) + "!");
        } else {
            iniciarTextoGradual(labelObjeto, "No tengo ningún objeto que darte.");
        }
        break;
    }

    default:
        btnSiguiente->setEnabled(false);
        emit interaccionFinalizada();  // ⬅ Emitimos la señal personalizada
        this->close();
        return;
    }

    currentStep++;
}

void MenuInteraccionPersonaje::verificarRespuesta() {
    QString respuesta = inputRespuesta->text().trimmed();
    if (respuesta.compare(personaje->getAcertijo()->getRespuesta(), Qt::CaseInsensitive) == 0) {
        mostrarDialogo(true);
    } else {
        mostrarDialogo(false);
    }
}

void MenuInteraccionPersonaje::mostrarDialogo(bool correcta) {
    if (correcta) {
        QMessageBox::information(this, "Respuesta Correcta", "¡Correcto! Ahora te daré una pista sobre el misterio...");
        btnSiguiente->setEnabled(true);
    } else {
        // Elegir una frase aleatoria de las incorrectas
        int index = QRandomGenerator::global()->bounded(frasesIncorrectas.size());
        QString frase = frasesIncorrectas.at(index);

        QMessageBox::information(this, "Respuesta Incorrecta", frase);
    }
}

void MenuInteraccionPersonaje::permitirNuevoIntento() {
    inputRespuesta->clear();
    inputRespuesta->setFocus();
}


void MenuInteraccionPersonaje::configurarFrasesIncorrectas() {
    // Lista de frases para cuando la respuesta es incorrecta
    frasesIncorrectas << "No es la respuesta correcta. ¡Te daré otra oportunidad!";
    frasesIncorrectas << "Hmm, no es eso. Vuelve a intentarlo.";
    frasesIncorrectas << "No, esa no es la respuesta. Intenta de nuevo.";
    frasesIncorrectas << "¡Casi! Pero aún no es correcto. Otro intento.";
}

void MenuInteraccionPersonaje::iniciarTextoGradual(QLabel* label, const QString& texto) {
    if (!label) return;

    if (timerTexto->isActive()) {
        timerTexto->stop();
    }

    labelActual = label;
    textoCompleto = texto;
    indexTexto = 0;

    labelActual->clear();
    scrollArea->setWidget(labelActual);
    labelActual->show();

    timerTexto->start(50);
}

void MenuInteraccionPersonaje::mostrarTextoGradualmente() {
    if (!labelActual) return;

    if (indexTexto < textoCompleto.length()) {
        labelActual->setText(labelActual->text() + textoCompleto[indexTexto]);
        indexTexto++;
    } else {
        timerTexto->stop();
    }
}

MenuInteraccionPersonaje::~MenuInteraccionPersonaje(){

}