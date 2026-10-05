#include "jugador.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace Usuario;

Jugador::Jugador(const char* nombre, const char* contrasena)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);

    this->contrasena = new char[strlen(contrasena)+1];
    strcpy(this->contrasena, contrasena);

    this->objetos = nullptr;
    this->cantObjetos = 0;
}

Jugador::Jugador(){
    this->nombre = nullptr;
    this->contrasena = nullptr;
    this->objetos = nullptr;
    this->cantObjetos = 0;
}

Jugador::~Jugador()
{
    delete this->nombre;
    delete this->contrasena;
}

char* Jugador::getNombre()
{
    return this->nombre;
}

char* Jugador::getContrasena()
{
    return this->contrasena;
}

char** Jugador::getObjetos()
{
    return this->objetos;
}

int Jugador::getCantObjetos()
{
    return this->cantObjetos;
}

void Jugador::setNombre(char* nombre)
{
    this->nombre = new char[strlen(nombre)+1];
    strcpy(this->nombre, nombre);
}

void Jugador::setContrasena(char* contrasena)
{
    this->contrasena = new char[strlen(contrasena)+1];
    strcpy(this->contrasena, contrasena);
}

void Jugador::imprimirJugador()
{
    cout<<"[Nombre: "<<this->nombre<<", Contrasena: "<<this->contrasena<<endl;

    if(this->cantObjetos != 0){
        cout<<", Objetos: "<<endl;

        for(int i = 0; i< this->cantObjetos; i++){
            cout<<this->objetos[i]<<endl;
        }

    }
    cout<<"]"<<endl;
}


void Jugador::anadirObjeto(const char* objeto)
{
    char** aux = new char*[this->cantObjetos + 1];
    for (int i = 0; i < this->cantObjetos; i++)
    {
        aux[i] = this->objetos[i];
    }
    
    delete this->objetos;
    this->objetos = aux;

    this->objetos[this->cantObjetos] = new char[strlen(objeto)+1];
    strcpy(this->objetos[this->cantObjetos], objeto);

    this->cantObjetos++;
}

char* Jugador::getObjeto(int index){
    return this->objetos[index];
}

char* Jugador::convertirAChar() {
    const char* sepCampos = "#J#";
    const char* sepObjetos = "%J%";

    // Valores seguros
    const char* safeNombre = nombre ? nombre : "";
    const char* safeContrasena = contrasena ? contrasena : "";

    // Calcular longitud total para los objetos
    int objetosLen = 0;
    for (int i = 0; i < cantObjetos; ++i) {
        if (objetos[i]) {
            objetosLen += strlen(objetos[i]);
            if (i < cantObjetos - 1)
                objetosLen += strlen(sepObjetos);
        }
    }

    // Longitud total = nombre + sep + contraseña + (si hay objetos: sep + objetos) + \0
    int totalLen = strlen(safeNombre) + strlen(sepCampos)
                 + strlen(safeContrasena)
                 + (cantObjetos > 0 ? strlen(sepCampos) + objetosLen : 0)
                 + 1;

    char* resultado = new char[totalLen];
    resultado[0] = '\0';  // Inicializar string vacío

    strcat(resultado, safeNombre);
    strcat(resultado, sepCampos);
    strcat(resultado, safeContrasena);

    if (cantObjetos > 0) {
        strcat(resultado, sepCampos);
        for (int i = 0; i < cantObjetos; ++i) {
            if (objetos[i]) {
                strcat(resultado, objetos[i]);
                if (i < cantObjetos - 1)
                    strcat(resultado, sepObjetos);
            }
        }
    }

    return resultado;
}


Jugador::Jugador(const char* linea) {
    const char* sepCampo = "#J#";
    const char* sepObjetos = "%J%";
    const int MAX_PARTES = 3;
    const char* partes[MAX_PARTES] = {nullptr};

    // Si la línea es nula, no inicializamos nada (o podrías llamar al constructor por defecto si lo deseas)
    if (!linea) {
        nombre = nullptr;
        contrasena = nullptr;
        objetos = nullptr;
        cantObjetos = 0;
        return;
    }

    const char* inicio = linea;
    int i = 0;

    // Separar por "#J#"
    while (i < MAX_PARTES - 1) {
        const char* pos = strstr(inicio, sepCampo);
        if (!pos) break;

        int len = pos - inicio;
        char* tmp = new char[len + 1];
        strncpy(tmp, inicio, len);
        tmp[len] = '\0';
        partes[i++] = tmp;

        inicio = pos + strlen(sepCampo);
    }

    if (*inicio != '\0') partes[i++] = strdup(inicio);

    // Si faltan nombre o contraseña, tratamos esto como una línea inválida
    if (i < 2 || !partes[0] || !partes[1]) {
        // Limpieza
        for (int j = 0; j < i; ++j)
            delete[] partes[j];

        nombre = nullptr;
        contrasena = nullptr;
        objetos = nullptr;
        cantObjetos = 0;
        return;
    }

    // Asignar campos válidos
    nombre = strdup(partes[0]);
    contrasena = strdup(partes[1]);

    // Inicializar objetos si hay
    objetos = nullptr;
    cantObjetos = 0;

    if (i == 3 && partes[2] && strlen(partes[2]) > 0) {
        const char* str = partes[2];
        int count = 1;

        for (const char* p = str; (p = strstr(p, sepObjetos)); p += strlen(sepObjetos))
            ++count;

        objetos = new char*[count];
        cantObjetos = count;

        char* copia = new char[strlen(str) + 1];
        strcpy(copia, str);

        char* ptr = copia;
        for (int j = 0; j < count; ++j) {
            char* next = strstr(ptr, sepObjetos);
            if (next) *next = '\0';

            objetos[j] = strdup(ptr);
            if (!next) break;
            ptr = next + strlen(sepObjetos);
        }

        delete[] copia;
    }

    // Limpieza temporal
    for (int j = 0; j < i; ++j)
        delete[] partes[j];
}


