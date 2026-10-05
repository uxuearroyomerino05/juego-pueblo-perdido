#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include "ficheros.h"

char* obtener_valor_config(const char* key) {
    char* resultado = nullptr;

    // 1. Ruta del archivo de configuración
    const char* ruta = "../../../Ficheros/ConfiguracionClient.txt";
    std::ifstream file(ruta);

    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo: " << ruta << std::endl;
        return nullptr;
    }

    // 2. Leer línea por línea
    std::string linea;
    while (std::getline(file, linea)) {
        // Verificar si la línea comienza con la clave
        if (linea.compare(0, std::strlen(key), key) == 0) {
            // Buscar el '='
            std::size_t pos_igual = linea.find('=');
            if (pos_igual != std::string::npos) {
                std::string valor = linea.substr(pos_igual + 1);

                // Reservar memoria y copiar el valor
                resultado = new char[valor.length() + 1];
                std::strcpy(resultado, valor.c_str());

                break;
            }
        }
    }

    file.close();
    return resultado;  
}

void escribirFicheroLog(const char* texto) {
    // 1. Obtener ruta base desde configuración
    char* basePath = obtener_valor_config("log_db_file_client");  
    if (basePath == nullptr) {
        std::cerr << "No se pudo obtener la ruta del log desde la configuración.\n";
        return;
    }

    // 2. Obtener fecha y hora actuales
    std::time_t t = std::time(nullptr);
    std::tm* tm_info = std::localtime(&t);

    char fecha[11];  // YYYY-MM-DD
    std::strftime(fecha, sizeof(fecha), "%Y-%m-%d", tm_info);

    char hora[9];    // HH:MM:SS
    std::strftime(hora, sizeof(hora), "%H:%M:%S", tm_info);

    // 3. Insertar la fecha antes de la extensión
    std::string baseName(basePath);
    delete[] basePath;

    std::string nombreArchivo;
    std::size_t punto = baseName.rfind('.');
    if (punto != std::string::npos) {
        nombreArchivo = baseName.substr(0, punto) + "_" + fecha + baseName.substr(punto);
    } else {
        nombreArchivo = baseName + "_" + fecha;
    }

    // 4. Abrir archivo en modo append
    std::ofstream file(nombreArchivo, std::ios::app);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo de log: " << nombreArchivo << "\n";
        return;
    }

    // 5. Escribir línea con hora
    file << "[" << hora << "] " << texto << std::endl;
}
