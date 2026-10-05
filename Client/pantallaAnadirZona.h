#ifndef PANTALLAANADIRZONA_H
#define PANTALLAANADIRZONA_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>

#include "personaje.h"
#include "zona.h"

using namespace Juego;

class PantallaAnadirZona : public QWidget {
    Q_OBJECT

public:
     PantallaAnadirZona(Zona* zona = nullptr, QWidget* parent = nullptr);
    ~PantallaAnadirZona();

signals:
    void zonaGuardado(const char* nombre, const char* descripcion, Personaje** personajes,
                      const int cantP);
    void volverAlMenu();

private:
    void actualizarListaPersonajes(int cantidad);
    void editarPersonaje(int indice);
    void asignarObjetoAPersonaje();
    void guardarZona();
    void cancelar();

    QLineEdit* inputNombreZona;
    QTextEdit* inputDescripcionZona;
    QLabel* lCantP;
    QSpinBox* spinCantidadPersonajes;
    QLineEdit* inputNombreObjeto;
    QLabel* labelPersonajeAsignado;
    QVBoxLayout* layoutPersonajes;
    QPushButton* btnGuardar;
    QPushButton* btnCancelar;

    std::vector<Personaje*> personajes;
    std::vector<QPushButton*> botonesPersonaje;
    int indicePersonajeAsignado;

    Zona* zonaActual;
};

#endif // PANTALLAANADIRZONA_H

