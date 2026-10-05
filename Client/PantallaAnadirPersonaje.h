#ifndef PANTALLAANADIRPERSONAJE_H
#define PANTALLAANADIRPERSONAJE_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "personaje.h"

using namespace Juego;

class PantallaAnadirPersonaje : public QWidget {
    Q_OBJECT

public:
    PantallaAnadirPersonaje(Personaje* personaje = nullptr, QWidget* parent = nullptr);
    ~PantallaAnadirPersonaje();

signals:
    void personajeGuardado(const char* nombre, const char* dialogo,
                           const char* pista, const char* pregunta,
                           const char* respuesta);
    void volverAlMenu();

private:
    void guardarPersonaje();
    void cancelar();

    QLineEdit* inputNombre;
    QTextEdit* inputDialogo;
    QTextEdit* inputPista;
    QLineEdit* inputPregunta;
    QLineEdit* inputRespuesta;

    QPushButton* btnGuardar;
    QPushButton* btnCancelar;

    void mostrarMensajeError(const QString& mensaje);

    Personaje* personajeActual;
};

#endif // PANTALLAANADIRPERSONAJE_H

