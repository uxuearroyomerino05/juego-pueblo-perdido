#ifndef SELECCIONPANTALLA_H
#define SELECCIONPANTALLA_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

class SeleccionPantalla : public QWidget
{
    Q_OBJECT

public:
    SeleccionPantalla(const char* tipo, char** nombres, int cantidad, QWidget *parent = nullptr);
    ~SeleccionPantalla();

private:
    QLabel* etiqueta;
    QVBoxLayout* layout;
    QScrollArea* scrollArea;

    void crearBotones(char** nombres, int cantidad, QVBoxLayout* scrollLayout);

signals:
    void seleccionada(int indice);  // Enviar la posición seleccionada
};

#endif // SELECCIONPANTALLA_H

