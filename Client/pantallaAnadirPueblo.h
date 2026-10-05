#ifndef PANTALLAANADIRPUEBLO_H
#define PANTALLAANADIRPUEBLO_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>

#include "zona.h"
#include "pueblo.h"

using namespace Juego;

class PantallaAnadirPueblo : public QWidget {
    Q_OBJECT

public:
     PantallaAnadirPueblo(Pueblo* pueblo = nullptr, QWidget* parent = nullptr);
    ~PantallaAnadirPueblo();

signals:
    void puebloGuardado(const char* nombre, const char* introduccion, const char* lugarMisterio,
                        const char* misterio, Zona** zonas, const int cantZonas);
    void volverAlMenu();

private:
    void actualizarListaZonas(int cantidad);
    void editarZona(int indice);
    void guardarPueblo();
    void cancelar();

    QLineEdit* inputNombrePueblo;
    QTextEdit* inputIntroduccion;
    QLineEdit* inputLugarMisterio;
    QTextEdit* inputMisterio;
    QLabel* lCantZ;
    QSpinBox* spinCantidadZonas;
    QVBoxLayout* layoutZonas;
    QPushButton* btnGuardar;
    QPushButton* btnCancelar;

    std::vector<Zona*> zonas;
    std::vector<QPushButton*> botonesZona;
    int indiceZonaAsignado;

    Pueblo* puebloActual;
};

#endif // PANTALLAANADIRPUEBLO_H

