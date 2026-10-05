#ifndef MENUINTERACCIONPERSONAJE_H
#define MENUINTERACCIONPERSONAJE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStringList>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QTimer>
#include <QScrollArea>
#include "personaje.h"

using namespace Juego;

class MenuInteraccionPersonaje : public QWidget {
    Q_OBJECT

public:
    MenuInteraccionPersonaje(QWidget* parent = nullptr, Personaje* personaje = nullptr);
    ~MenuInteraccionPersonaje();

private:
    QLabel* labelPersonaje;
    QLabel* labelDialogo;
    QLabel* labelAcertijo;
    QLabel* labelPista;
    QLabel* labelObjeto;
    QLineEdit* inputRespuesta;
    QPushButton* btnSiguiente;
    QPushButton* btnEnviar;
    QScrollArea* scrollArea;

    QTimer* timerTexto;
    QString textoCompleto;
    QLabel* labelActual;
    int indexTexto;

    Personaje* personaje;
    QStringList frasesIncorrectas;
    QStringList dialogoList;  // Para el diálogo
    int currentStep;          // Controla en qué paso estamos (diálogo, pregunta, pista, objeto)

    void mostrarSiguientePantalla();
    void configurarFrasesIncorrectas();
    void verificarRespuesta();
    void mostrarDialogo(bool correcta);
    void permitirNuevoIntento();
    void mostrarTextoGradualmente();
    void iniciarTextoGradual(QLabel* label, const QString& texto);

signals:
    void interaccionFinalizada();

};

#endif // MENUINTERACCIONPERSONAJE_H
