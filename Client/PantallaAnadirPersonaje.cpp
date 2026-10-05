#include "PantallaAnadirPersonaje.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>

PantallaAnadirPersonaje::PantallaAnadirPersonaje(Personaje* personaje, QWidget* parent) : QWidget(parent) {

    this->personajeActual = personaje;

    // Escalado relativo
    QSize screenSize = QApplication::primaryScreen()->size();
    int screenHeight = screenSize.height();
    int fontSize = screenHeight / 35;
    int labelFontSize = screenHeight / 40;
    int inputHeight = screenHeight / 20;
    int buttonHeight = screenHeight / 12;

    // Estilo de fuente común
    QFont fuente("Verdana", fontSize);
    QFont fuenteLabel("Verdana", labelFontSize);

    // Widgets
    inputNombre = new QLineEdit(this);
    inputNombre->setFont(fuente);
    inputNombre->setMinimumHeight(inputHeight);

    inputDialogo = new QTextEdit(this);
    inputDialogo->setFont(fuente);
    inputDialogo->setMinimumHeight(inputHeight * 2);

    inputPista = new QTextEdit(this);
    inputPista->setFont(fuente);
    inputPista->setMinimumHeight(inputHeight * 2);

    inputPregunta = new QLineEdit(this);
    inputPregunta->setFont(fuente);
    inputPregunta->setMinimumHeight(inputHeight);

    inputRespuesta = new QLineEdit(this);
    inputRespuesta->setFont(fuente);
    inputRespuesta->setMinimumHeight(inputHeight);

    btnGuardar = new QPushButton("Guardar", this);
    btnGuardar->setFont(fuente);
    btnGuardar->setMinimumHeight(buttonHeight);

    btnCancelar = new QPushButton("Cancelar", this);
    btnCancelar->setFont(fuente);
    btnCancelar->setMinimumHeight(buttonHeight);

    connect(btnGuardar, &QPushButton::clicked, this, &PantallaAnadirPersonaje::guardarPersonaje);
    connect(btnCancelar, &QPushButton::clicked, this, &PantallaAnadirPersonaje::cancelar);

    // Layouts
    QVBoxLayout* layout = new QVBoxLayout(this);

    auto addCampo = [&](const QString& texto, QWidget* widget) {
        QLabel* label = new QLabel(texto, this);
        label->setFont(fuenteLabel);
        layout->addWidget(label);
        layout->addWidget(widget);
    };

    addCampo("Nombre del personaje:", inputNombre);
    addCampo("Diálogo:", inputDialogo);
    addCampo("Pista:", inputPista);
    addCampo("Pregunta del acertijo:", inputPregunta);
    addCampo("Respuesta del acertijo:", inputRespuesta);

    QHBoxLayout* layoutBotones = new QHBoxLayout();
    layoutBotones->addWidget(btnCancelar);
    layoutBotones->addWidget(btnGuardar);

    layout->addSpacing(screenHeight / 40);
    layout->addLayout(layoutBotones);

    setLayout(layout);
    setWindowTitle("Añadir nuevo personaje");

    if (personajeActual != nullptr) {
        inputNombre->setText(personajeActual->getNombre());
        inputDialogo->setText(personajeActual->getDialogo());
        inputPista->setText(personajeActual->getPista());
        inputPregunta->setText(personajeActual->getAcertijo()->getPregunta());
        inputRespuesta->setText(personajeActual->getAcertijo()->getRespuesta());
        setWindowTitle("Modificar personaje");
    } else {
        setWindowTitle("Añadir nuevo personaje");
    }

    showFullScreen();
}

void PantallaAnadirPersonaje::guardarPersonaje() {
    QString nombre = inputNombre->text().trimmed();
    QString dialogo = inputDialogo->toPlainText().trimmed();
    QString pista = inputPista->toPlainText().trimmed();
    QString pregunta = inputPregunta->text().trimmed();
    QString respuesta = inputRespuesta->text().trimmed();

    if (nombre.isEmpty() || dialogo.isEmpty() || pista.isEmpty() || pregunta.isEmpty() || respuesta.isEmpty()) {
        mostrarMensajeError("Todos los campos son obligatorios.");
        return;
    }

    emit personajeGuardado(
        nombre.toStdString().c_str(),
        dialogo.toStdString().c_str(),
        pista.toStdString().c_str(),
        pregunta.toStdString().c_str(),
        respuesta.toStdString().c_str()
    );

    close();
}

void PantallaAnadirPersonaje::cancelar() {
    emit volverAlMenu();
    close();
}

void PantallaAnadirPersonaje::mostrarMensajeError(const QString& mensaje) {
    QMessageBox::warning(this, "Error", mensaje);
}

PantallaAnadirPersonaje::~PantallaAnadirPersonaje(){

}