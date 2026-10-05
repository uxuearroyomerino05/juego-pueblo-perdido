#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ficheros.h"

/**
 * Función para obtener un valor del archivo de configuración.
 * 
 * @param key Clave a buscar en el archivo.
 * @return Puntero a una cadena con el valor asociado a la clave (debe liberarse con free).
 */
char* obtener_valor_config(const char* key) {
    char* resultado = NULL;

    // 1. Definir la ruta del archivo de configuración
    char ruta[] = "../Ficheros/ConfiguracionServer.txt";

    // 2. Abrir el archivo en modo lectura
    FILE* file = fopen(ruta, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo: %s\n", ruta);
        return NULL;  // Si no se puede abrir el archivo, retornar NULL
    }

    // 3. Leer línea por línea buscando la clave
    int c;
    char linea[256]; // Buffer
    int i = 0; // Índice buffer
    int seguir = 0;

    while (seguir != 1) { // Leer hasta el final
        c = fgetc(file);

        if (c == '\n' || c == EOF || i == 256 - 1) {
            linea[i] = '\0';  // Fin de cadena

            // Comprobar la key (clave)
            if (strncmp(linea, key, strlen(key)) == 0) {

                // 4. Si se encuentra la clave:

                //    - Ubicar la posición del '='
                char* pos_igual = strchr(linea, '=');
                
                if (pos_igual) {
                    //    - Determinar la longitud del valor
                    char* pos_valor = pos_igual + 1;  // Apuntar al primer carácter del valor
                    int longitud_valor = strlen(pos_valor); // Obtener la longitud del valor

                    //    - Reservar memoria con malloc
                    resultado = malloc(longitud_valor + 1); // +1 para el '\0'

                    //    - Copiar el valor en la memoria reservada
                    strcpy(resultado, pos_valor);

                    break;  // Si ya encontramos la clave, no necesitamos seguir buscando
                }
            }

            i = 0;  // Reiniciar índice
        } else {
            linea[i++] = c;  // Guardar carácter en el buffer
        }

        if(c == EOF){
            seguir = 1;
        }
    }

    // 5. Cerrar el archivo
    fclose(file);

    // 6. Devolver el valor. Si no se encuentra la clave, devolver NULL
    return resultado;
}

void obtener_fecha_actual(char* buffer, size_t buffer_size) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, buffer_size, "%Y-%m-%d", tm_info);  // Para el nombre del archivo
}

void obtener_hora_actual(char* buffer, size_t buffer_size) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buffer, buffer_size, "%H:%M:%S", tm_info);  // Para cada línea del log
}

void escribirFicheroLog(const char* texto) {
    // 1. Obtener nombre base desde configuración
    char* nombre_base = obtener_valor_config("log_db_file_server");
    if (nombre_base == NULL) {
        printf("No se pudo obtener el nombre del fichero del log\n");
        return;
    }

    // 2. Obtener fecha actual (para el nombre del archivo)
    char fecha[11];
    obtener_fecha_actual(fecha, sizeof(fecha));

    // 3. Construir nombre del archivo con la fecha
    char nombre_final[256];
    char* punto = strrchr(nombre_base, '.');
    if (punto != NULL) {
        size_t nombre_len = punto - nombre_base;
        snprintf(nombre_final, sizeof(nombre_final), "%.*s_%s%s",
                 (int)nombre_len, nombre_base, fecha, punto);
    } else {
        snprintf(nombre_final, sizeof(nombre_final), "%s_%s", nombre_base, fecha);
    }

    // 4. Abrir archivo en modo append
    FILE* file = fopen(nombre_final, "a");
    if (file == NULL) {
        printf("No se pudo abrir el archivo: %s\n", nombre_final);
        free(nombre_base);
        return;
    }

    // 5. Obtener hora actual para la línea de log
    char hora[9];  // HH:MM:SS
    obtener_hora_actual(hora, sizeof(hora));

    // 6. Escribir línea con hora
    fprintf(file, "[%s] %s\n", hora, texto);

    // 7. Cerrar y liberar
    fclose(file);
    free(nombre_base);
}
