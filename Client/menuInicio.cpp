#include "menuInicio.h"
#include "PantallaMenuAdministrador.h"
#include "menuPrincipal.h"
#include "seleccionPantalla.h"
#include "pueblo.h"
#include "iniciarSesion.h"
#include "registrarse.h"
#include <QFont>
#include <QDebug>
#include <QResizeEvent>
#include <QMessageBox>
#include "ficheros.h"

#include "protocoloMensages.h"
#include <winsock2.h>

#include <iostream>

#include <string.h>

using namespace Juego;
using namespace Usuario;

MenuInicio::MenuInicio( const SOCKET s, QWidget *parent) :
    QWidget(parent),
    titulo(new QLabel("Bienvenido al juego", this)),
    btnIniciarSesion(new QPushButton("Iniciar sesión", this)),
    btnRegistrarse(new QPushButton("Registrarse", this)),
    btnSalir(new QPushButton("Salir", this))
{
    this->s = s;

    // Configurar el layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(titulo);
    layout->addWidget(btnIniciarSesion);
    layout->addWidget(btnRegistrarse);
    layout->addWidget(btnSalir);

    // Establecer un tamaño mínimo
    setMinimumSize(800, 600);

    // Mostrar la ventana en pantalla completa
    showFullScreen();

    // Conectar las señales a las acciones correspondientes
    connect(btnIniciarSesion, &QPushButton::clicked, this, [this, s]() {
        IniciarSesion *login = new IniciarSesion(s, this);
        connect(login, &IniciarSesion::usuarioaGuardado, this, &MenuInicio::guardarUsuario );
        login->exec(); // Mostrar ventana de login
        delete login;

        // Llamamos a la función empezarJuego después del registro
        if (strcmp(usuario->getNombre(), obtener_valor_config("admin_user"))==0 && strcmp(usuario->getContrasena(), obtener_valor_config("admin_contrasena")) == 0) {
            PantallaMenuAdministrador* menuAdmin = new PantallaMenuAdministrador(this->s);
            menuAdmin->show();
        } else {
            empezarJuego();
        }
        
        // Cerraresta ventana (menuInicio)
        this->close();

    });

    connect(btnRegistrarse, &QPushButton::clicked, this, [this, s]() {
        Registrase *login = new Registrase(s, this);
        connect(login, &Registrase::usuarioaGuardado, this, &MenuInicio::guardarUsuario );
        login->exec(); // Mostrar ventana de login
        delete login;

        // Llamamos a la función empezarJuego después del registro
        empezarJuego();  

    });

    connect(btnSalir, &QPushButton::clicked, this, [this, s]() {
        char* recv = nullptr;
        char comando[] =   "Bye";
        enviarYRecibir(s, comando, &recv);
        delete[] recv; 
    
        closesocket(s);
        WSACleanup();
    
        this->close(); // Cierra la ventana
    });
    

    // Personalización del botón de cerrar (salir)
    QPalette palette = btnSalir->palette();
    palette.setColor(QPalette::Button, Qt::red);  // Botón rojo para salir
    btnSalir->setAutoFillBackground(true);
    btnSalir->setPalette(palette);
    btnSalir->setStyleSheet("color: white; font-weight: bold;");

}

MenuInicio::~MenuInicio()
{

}

void MenuInicio::resizeEvent(QResizeEvent *event) {
    // Escalar el tamaño de la fuente en función del tamaño de la ventana
    int fontSizeTitulo = qMin(width(), height()) / 10; // Ajustamos el tamaño del título para hacerlo más grande
    int fontSizeBotones = qMin(width(), height()) / 20; // Tamaño de los botones

    // Ajustar la fuente del título
    QFont fontTitulo = titulo->font();
    fontTitulo.setPointSize(fontSizeTitulo); // Ajustar el tamaño de la fuente del título
    fontTitulo.setBold(true); // Hacer el título en negrita
    fontTitulo.setFamily("Comic Sans MS"); // Fuente diferente para el título
    titulo->setFont(fontTitulo);
    titulo->setAlignment(Qt::AlignCenter); // Centrar el título

    // Ajustar la fuente de los botones
    QFont fontBotones = btnIniciarSesion->font();
    fontBotones.setPointSize(fontSizeBotones); // Ajustar el tamaño de la fuente de los botones
    fontBotones.setFamily("Verdana"); // Fuente para los botones
    btnIniciarSesion->setFont(fontBotones);
    btnRegistrarse->setFont(fontBotones);
    btnSalir->setFont(fontBotones);

    // Llamar al manejador del evento original
    QWidget::resizeEvent(event);
}

void MenuInicio::empezarJuego()
{
    // LLAMAR SERVIDOR: GET_NOMBRES_PUEBLOS
    char* recv = nullptr;
    char coman[] = "GET_NOMBRES_PUEBLO";
    enviarYRecibir(s, coman, &recv);

    char** nombresPueblos = nullptr;
    int cantidad = 0;

    if (recv != nullptr) {
        char* token = strtok(recv, "%L%");
        while (token != nullptr) {
            // Reservar nuevo array con espacio adicional
            char** aux = new char*[cantidad + 1];
            
            // Copiar punteros anteriores
            for (int i = 0; i < cantidad; i++) {
                aux[i] = nombresPueblos[i];
            }

            // Agregar nuevo token
            aux[cantidad] = new char[strlen(token) + 1];
            strcpy(aux[cantidad], token);
            cantidad++;

            // Liberar array anterior
            delete nombresPueblos;
            nombresPueblos = aux;

            // Obtener siguiente token
            token = strtok(nullptr, "%L%");
        }
    }

    delete[] recv;   

    // Crear la pantalla de selección de pueblos
    SeleccionPantalla* seleccionPueblos = new SeleccionPantalla("Pueblo", nombresPueblos, cantidad);
    connect(seleccionPueblos, &SeleccionPantalla::seleccionada, this, [=](int indice) {

        //LLAMAR SERVIDOR: GET_PUEBLO
        char* recv = nullptr;
        char* comando = new char[11+strlen(nombresPueblos[indice])+1];
        strcpy(comando, "GET_PUEBLO|");
        strcat(comando, nombresPueblos[indice]);
        enviarYRecibir(s, comando, &recv);

        Pueblo* pueblo = new Pueblo(recv);
        delete[] recv;
        delete[] comando;

        // LLAMAR SERVIDOR: ANADIR_OBJETOS_JUGADOR
        char* recv2 = nullptr;
        char* comando1 = new char[24+strlen(this->usuario->convertirAChar())+1+strlen(pueblo->getNombre())+1];
        strcpy(comando1, "ANADIR_OBJETOS_JUGADOR|");
        strcat(comando1, this->usuario->convertirAChar());
        strcat(comando1, "|");
        strcat(comando1, pueblo->getNombre());
        enviarYRecibir(s, comando1, &recv2);
        delete this->usuario;
        this->usuario = new Jugador(recv2);
        delete[] recv2;

        // Abrir el menu de juego inicial
        MenuPrincipal* menuPrincipal = new MenuPrincipal(usuario, pueblo, s);
        menuPrincipal->show();
        seleccionPueblos->close();  // Cerrar la pantalla de selección
        this->close(); // Cerrar esta ventana (menuInicio)
    });
    seleccionPueblos->show();

}

void MenuInicio::guardarUsuario(Jugador* usuario){
    this->usuario = usuario;
}