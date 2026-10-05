#include "ficheros.h"
#include "baseDeDatos.h"
#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "zona_Struct.h"
#include "personaje_Struct.h"
#include "acertijo_Struct.h"
#include "jugador_Struct.h"
#include "pueblo_Struct.h"

void modificarContrasena(char* nombreJugador, char* nueva_contrasena) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;

    // Abrir la base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    // Sentencia SQL para actualizar la contraseña
    char sql[] = "UPDATE Jugador SET contrasena = ? WHERE nombre = ?";

    // Preparar la sentencia SQL
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, nueva_contrasena, strlen(nueva_contrasena), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombreJugador, strlen(nombreJugador), SQLITE_STATIC);

    // Ejecutar la sentencia SQL
    result = sqlite3_step(stmt);

    // Comprobar que la actualización se realizó correctamente
    if (result != SQLITE_DONE) {
        printf("Error al modificar la contrasena del jugador con nombre %s\n", nombreJugador);
    } else {
        printf("Contrasena del jugador con nombre %s modificada correctamente\n", nombreJugador);
    }

    // Cerrar la sentencia y la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}

void insertarPueblo(Pueblo_Struct p){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "insert into Pueblo (nombre, introduccion, lugarMisterio, misterio ) values (?, ?, ?, ?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, p.nombre, strlen(p.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, p.introduccion, strlen(p.introduccion), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p.lugarMisterio, strlen(p.lugarMisterio), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, p.misterio, strlen(p.misterio), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Insertar las zonas
    for(int i = 0; i<p.cantZonas; i++){
        insertarZona(p.zonas[i], p.nombre);
    }

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error insertando el pueblo\n");
	}else{
		printf("Pueblo %s insertado\n", p.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void insertarZona(Zona_Struct z, char* nombrePueblo){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "insert into Zona (nombre, descripcion, idPueblo) values (?, ?, ?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, z.nombre, strlen(z.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, z.descripcion, strlen(z.descripcion), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nombrePueblo, strlen(nombrePueblo), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Insertar las Personaje
    for(int i = 0; i<z.cantP; i++){
        insertarPersonaje(z.personajes[i], z.nombre);
    }

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error insertando el zona\n");
	}else{
		printf("Zona %s insertado\n", z.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void insertarPersonaje(Personaje_Struct p, const char* nombreZona){

    //Acertijo
    insertarAcertijo(p.acertijo);
    int idAcertijo = obtnerIdAcertijoPorPregunta(p.acertijo.pregunta);

    //Insertar objeto si hay
    int idObjeto;
    int tieneObjeto = 0;
    if(p.objeto != NULL){
        insertarObjeto(p.objeto, nombreZona);
        idObjeto = obtnerIdObjetoPorNombre(p.objeto);
        tieneObjeto = 1;
    }
    
    //Datos Zona
    int idZona = obtnerIdZonaPorNombre(nombreZona);

    //Insertar Personaje ---------------------------

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "insert into Personaje (nombre, dialogo, pista, id_acertijo, id_objeto, id_zona ) values (?, ?, ?, ?, ?, ?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, p.nombre, strlen(p.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, p.dialogo, strlen(p.dialogo), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p.pista, strlen(p.pista), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, idAcertijo);
    if (tieneObjeto){
        sqlite3_bind_int(stmt, 5, idObjeto);
    } else {
        sqlite3_bind_null(stmt, 5);
    }
    sqlite3_bind_int(stmt, 6, idZona);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error insertando el personaje\n");
	}else{
		printf("Personaje %s insertado\n", p.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void insertarAcertijo(Acertijo_Struct a){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "Insert into Acertijo (pregunta, respuesta) values (?, ?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, a.pregunta, strlen(a.pregunta), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, a.respuesta, strlen(a.respuesta), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error insertando el acertijo\n");
	}else{
		printf("Acertijo %s insertado\n", a.pregunta);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void insertarObjeto(char* o, const char* nombreZona){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "insert into Objeto (id, nombre) values (?,?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
    sqlite3_bind_int(stmt, 1, obtnerIdZonaPorNombre(nombreZona));
	sqlite3_bind_text(stmt, 2, o, strlen(o), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error insertando el objeto\n");
	}else{
		printf("Objeto %s insertado\n", o);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void insertarJugador(Jugador_Struct j){
    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "insert into Jugador (nombre, contrasena) values (?, ?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, j.nombre, strlen(j.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, j.contrasena, strlen(j.contrasena), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error insertando el jugador\n");
	}else{
		printf("Jugador %s(%s) insertado\n", j.nombre, j.contrasena);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);
}

void insertarObjetoJugador(char* nombreJugador, char* nombreObjeto, char* nombrePueblo){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;
    int idObjeto = obtnerIdObjetoPorNombre(nombreObjeto);

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "insert into JugadorObjetos (nombreJugador, idObjeto, idPueblo) values (?, ?, ?)";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, nombreJugador, strlen(nombreJugador), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idObjeto);
    sqlite3_bind_text(stmt, 3, nombrePueblo, strlen(nombrePueblo), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error al insertar en la tabla intermedia de jugador y objeto\n");
	}else{
		printf("En la tabla intermedia de jugador y objeto  %s(%i) insertado\n", nombreJugador, idObjeto);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

char** obtenerNombresPueblo(int* cantP){

    char* rutaBD = obtener_valor_config("db_file");
    if (rutaBD == NULL) {
        printf("Error: no se pudo leer la ruta de la base de datos\n");
        return NULL;
    }

    sqlite3* db;
    sqlite3_stmt *stmt;
    int result;
    char** nombres = NULL;
    *cantP = 0;
    
    int rc = sqlite3_open(rutaBD, &db);
    if (rc != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(rutaBD);
        return NULL;
    }

    //Sentencia sql
    char sql[] = "select * from Pueblo";

    //Preparar sentencia
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	//Recorrer los resultados
	do {
		result = sqlite3_step(stmt) ;

		if (result == SQLITE_ROW) {

            //Ampliar la array 
            char** aux = malloc(sizeof(char**)*(*cantP+1));

            for(int  i = 0; i<(*cantP); i++){
                aux[i] = nombres[i];
            }

            free(nombres);
            nombres = aux;

            //Anadir Nombre
			char* nombre = (char*)sqlite3_column_text(stmt, 0);
            nombres[*cantP] = malloc(strlen(nombre) + 1);
            strcpy(nombres[*cantP], nombre);

            //Aumentar el contador de nombres
            (*cantP)++;
		}
	} while (result == SQLITE_ROW);

    //Cerrar select
	sqlite3_finalize(stmt);
    
    //Cerrar base de datos
    sqlite3_close(db);

    return nombres;

}

Pueblo_Struct obtenerPueblo(char* nombre){

    Pueblo_Struct pueblo;

    char* rutaBD = obtener_valor_config("db_file");
    if (rutaBD == NULL) {
        printf("Error: no se pudo leer la ruta de la base de datos\n");
    } else {
        sqlite3 *db;
        sqlite3_stmt *stmt;
        int result;
        
        int rc = sqlite3_open(rutaBD, &db);
        if (rc != SQLITE_OK) {
            printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            free(rutaBD);
        } else {
            //Sentencia sql
            char sql[] = "select * from Pueblo where nombre = ?";

            //Preparar sentencia
            sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
            sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);

            //Recuperar el resultados
            result = sqlite3_step(stmt);
                
            if (result == SQLITE_ROW) {

                char* nombre1 = (char*)sqlite3_column_text(stmt, 0);
                pueblo.nombre = malloc(strlen(nombre1) + 1);
                strcpy(pueblo.nombre, nombre1);

                char* introduccion = (char*)sqlite3_column_text(stmt, 1);
                pueblo.introduccion = malloc(strlen(introduccion) + 1);
                strcpy(pueblo.introduccion, introduccion);

                char* lugarMisterio = (char*)sqlite3_column_text(stmt, 2);
                pueblo.lugarMisterio = malloc(strlen(lugarMisterio) + 1);
                strcpy(pueblo.lugarMisterio, lugarMisterio);

                char* misterio = (char*)sqlite3_column_text(stmt, 3);
                pueblo.misterio = malloc(strlen(misterio) + 1);
                strcpy(pueblo.misterio, misterio);

                //Recuperar Zonas
                pueblo.cantZonas = 0;
                pueblo.zonas = obtenerZonas(&(pueblo.cantZonas), pueblo.nombre);
                    
            } 

            //Cerrar select
            sqlite3_finalize(stmt);
                
            //Cerrar base de datos
            sqlite3_close(db);

        }

    }

    return pueblo;

}

Zona_Struct* obtenerZonas(int* cantZ, char* idPueblo){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    Zona_Struct* zonas = NULL; 
    *cantZ = 0;

    //Abrrir base de datos
    if (sqlite3_open(obtener_valor_config("db_file"), &db) != SQLITE_OK) {
        return NULL;
    }

    //Sentencia sql
    char sql[] = "select * from Zona where idPueblo = ?";

    //Preparar sentencia
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, idPueblo, strlen(idPueblo), SQLITE_STATIC);

	//Recorrer los resultados
	do {
		result = sqlite3_step(stmt) ;

		if (result == SQLITE_ROW) {

            // Ampliar el array de personajes con realloc
            zonas = (Zona_Struct*) realloc(zonas, sizeof(Zona_Struct) * (*cantZ + 1));
            if (zonas == NULL) {
                // Manejo de error si realloc falla
                printf("Error al ampliar memoria para zonas\n");
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return NULL;
            }

            //Anadir Zona

			char* nombre = (char*)sqlite3_column_text(stmt, 1);
            zonas[*cantZ].nombre = malloc(strlen(nombre) + 1);
            strcpy(zonas[*cantZ].nombre, nombre);

            char* descripcion = (char*)sqlite3_column_text(stmt, 2);
            zonas[*cantZ].descripcion = malloc(strlen(descripcion) + 1);
            strcpy(zonas[*cantZ].descripcion, descripcion);

            //Recuperar personajes
            zonas[*cantZ].cantP = 0;
            zonas[*cantZ].personajes = obtenerPersonajesPorIdZona(sqlite3_column_int(stmt, 0), &(zonas[*cantZ].cantP));
            
            //Aumentar el contador de zonas
            (*cantZ)++;
		}
	} while (result == SQLITE_ROW);

    
    //Cerrar select
	sqlite3_finalize(stmt);
    
    //Cerrar base de datos
    sqlite3_close(db);

    return zonas;
}

Personaje_Struct* obtenerPersonajesPorIdZona(int id, int* cantP){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    Personaje_Struct* personaje = NULL; 
    *cantP = 0;

    //Abrrir base de datos
    if (sqlite3_open(obtener_valor_config("db_file"), &db) == SQLITE_OK) {
        
        //Sentencia sql
        char sql[] = "select * from Personaje where id_zona = ?";

        //Preparar sentencia
        sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
        sqlite3_bind_int(stmt, 1, id);

        //Recorrer los resultados
        do{
            result = sqlite3_step(stmt) ;

            if (result == SQLITE_ROW) {

                // Ampliar el array de personajes con realloc
                personaje = (Personaje_Struct*) realloc(personaje, sizeof(Personaje_Struct) * (*cantP + 1));
                if (personaje == NULL) {
                    // Manejo de error si realloc falla
                    printf("Error al ampliar memoria para personajes\n");
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return NULL;
                }

                //Anadir nueva personaje
                char* nombre = (char*)sqlite3_column_text(stmt, 1);
                personaje[*cantP].nombre = malloc(strlen(nombre) + 1);
                strcpy(personaje[*cantP].nombre, nombre);

                char* dialogo = (char*)sqlite3_column_text(stmt, 2);
                personaje[*cantP].dialogo = malloc(strlen(dialogo) + 1);
                strcpy(personaje[*cantP].dialogo, dialogo);

                char* pista = (char*)sqlite3_column_text(stmt, 3);
                personaje[*cantP].pista = malloc(strlen(pista) + 1);
                strcpy(personaje[*cantP].pista, pista);

                personaje[*cantP].acertijo = obtenerAcertijoPorId(sqlite3_column_int(stmt, 4));
                if(sqlite3_column_int(stmt, 5) != 0) {
                    personaje[*cantP].objeto = obtenerObjetoPorId(sqlite3_column_int(stmt, 5));
                } else {
                    personaje[*cantP].objeto = NULL;
                }
                
                (*cantP)++;
           
            }
        }while(result == SQLITE_ROW);

        //Cerrar select
	    sqlite3_finalize(stmt);
    
        //Cerrar base de datos
        sqlite3_close(db);

    }

    return personaje;
}

Acertijo_Struct obtenerAcertijoPorId(int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    Acertijo_Struct acertijo;

    //Abrrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "select * from Acertijo where id = ?";

    //Preparar sentencia
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

	//Recuperar el resultados

	result = sqlite3_step(stmt);

	if (result == SQLITE_ROW) {

        char* pregunta = (char*)sqlite3_column_text(stmt, 1);
        acertijo.pregunta = malloc(strlen(pregunta) + 1);
        strcpy(acertijo.pregunta, pregunta);

        char* respuesta = (char*)sqlite3_column_text(stmt, 2);
        acertijo.respuesta = malloc(strlen(respuesta) + 1);
        strcpy(acertijo.respuesta, respuesta);
		
	}

    //Cerrar select
	sqlite3_finalize(stmt);
    
    //Cerrar base de datos
    sqlite3_close(db);

    return acertijo;
}

Jugador_Struct obetenerJugadorPorNombre(char* nombre){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    Jugador_Struct jugador;

    //Abrrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "select * from Jugador where nombre = ?";

    //Preparar sentencia
    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);

    //Recuperar el resultados
    result = sqlite3_step(stmt);
        
    if (result == SQLITE_ROW) {

        char* nombre1 = (char*)sqlite3_column_text(stmt, 0);
        jugador.nombre = malloc(strlen(nombre1) + 1);
        strcpy(jugador.nombre, nombre1);

        char* contrasena = (char*)sqlite3_column_text(stmt, 1);
        jugador.contrasena = malloc(strlen(contrasena) + 1);
        strcpy(jugador.contrasena, contrasena);
         
    } else {
        jugador.nombre = NULL;
        jugador.contrasena = NULL;
    }

    jugador.objetos = NULL;
    jugador.cantObjetos = 0;

    //Cerrar select
    sqlite3_finalize(stmt);
        
    //Cerrar base de datos
    sqlite3_close(db);
    return jugador;
}

char* obtenerObjetoPorId(int id){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    char* objeto;

    //Abrrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "select * from Objeto where id = ?";

    //Preparar sentencia
	sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

	//Recuperar el resultados

	result = sqlite3_step(stmt);

	if (result == SQLITE_ROW) {

        char* objetoR = (char*)sqlite3_column_text(stmt, 1);
        objeto = malloc(strlen(objetoR) + 1);
        strcpy(objeto, objetoR);
		
	}

    //Cerrar select
	sqlite3_finalize(stmt);
    
    //Cerrar base de datos
    sqlite3_close(db);

    return objeto;
}

void obtenerObjetosPorIdJugadorPorIdPueblo(Jugador_Struct *j, char* idPueblo){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;

    //Abrrir base de datos
    if (sqlite3_open(obtener_valor_config("db_file"), &db) == SQLITE_OK) {

        //Sentencia sql
        char sql[] = "select * from JugadorObjetos where nombreJugador = ? AND idPueblo = ?";

        //Preparar sentencia
        sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
        sqlite3_bind_text(stmt, 1, j->nombre, strlen(j->nombre), SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, idPueblo, strlen(idPueblo), SQLITE_STATIC);

        //Recorrer los resultados
        do{
            result = sqlite3_step(stmt) ;

            if (result == SQLITE_ROW) {
                //Añadir objeto nuevo
                anadirObjeto_Struct(j, obtenerObjetoPorId(sqlite3_column_int(stmt, 1)));
           
            }
        }while(result == SQLITE_ROW);

        //Cerrar select
	    sqlite3_finalize(stmt);
    
        //Cerrar base de datos
        sqlite3_close(db);

    }
    
}

int obtnerIdObjetoPorNombre(char* nombre){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    int id = -1;

    //Abrrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "select * from Objeto where nombre = ?";

    //Preparar sentencia
    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);

    //Recuperar el resultados
    result = sqlite3_step(stmt);
        
    if (result == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }

    //Cerrar select
    sqlite3_finalize(stmt);
        
    //Cerrar base de datos
    sqlite3_close(db);

    return id;
}

int obtnerIdAcertijoPorPregunta(char* pregunta){

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    int id;

    //Abrrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "select * from Acertijo where pregunta = ?";

    //Preparar sentencia
    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, pregunta, strlen(pregunta), SQLITE_STATIC);

    //Recuperar el resultados
    result = sqlite3_step(stmt);
        
    if (result == SQLITE_ROW) {

        id = sqlite3_column_int(stmt, 0);
            
    } 

    //Cerrar select
    sqlite3_finalize(stmt);
        
    //Cerrar base de datos
    sqlite3_close(db);

    return id;

}

int obtnerIdZonaPorNombre(const char* nombre){

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    int id = -1;

    //Abrrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "select * from Zona where nombre = ?";

    //Preparar sentencia
    sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);

    //Recuperar el resultados
    result = sqlite3_step(stmt);
        
    if (result == SQLITE_ROW) {

        id = sqlite3_column_int(stmt, 0);
            
    } 

    //Cerrar select
    sqlite3_finalize(stmt);
        
    //Cerrar base de datos
    sqlite3_close(db);
    return id;

}

void modificarPueblo(Pueblo_Struct p, Pueblo_Struct puebloAnterior){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "update Pueblo set nombre=?, introduccion=?, lugarMisterio=?, misterio=? where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, p.nombre, strlen(p.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, p.introduccion, strlen(p.introduccion), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p.lugarMisterio, strlen(p.lugarMisterio), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, p.misterio, strlen(p.misterio), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, puebloAnterior.nombre, strlen(puebloAnterior.nombre), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error modificado el pueblo\n");
	}else{
		printf("Pueblo %s modificado\n", p.nombre);
	}

    //Insertar, modificar o eliminar las zonas
    if(puebloAnterior.cantZonas<p.cantZonas){
        for(int i = 0; i<puebloAnterior.cantZonas; i++){
            modificarZona(p.zonas[i], puebloAnterior.zonas[i]);
        }
        for(int i = puebloAnterior.cantZonas; i<p.cantZonas; i++){
            insertarZona(p.zonas[i], p.nombre);
        }
    } else {
        for(int i = 0; i<p.cantZonas; i++){
            modificarZona(p.zonas[i], puebloAnterior.zonas[i]);
        }
        for(int i = p.cantZonas; i<puebloAnterior.cantZonas; i++){
            eliminarZona(puebloAnterior.zonas[i]);
        }
    }

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void modificarZona(Zona_Struct z, Zona_Struct zonaAnterior){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "update Zona set nombre=?, descripcion = ? where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, z.nombre, strlen(z.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, z.descripcion, strlen(z.descripcion), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, zonaAnterior.nombre, strlen(zonaAnterior.nombre), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error modificado el zona\n");
	}else{
		printf("Zona %s modificado\n", z.nombre);
	}

    //Modificar los Personajes
    if(zonaAnterior.cantP<z.cantP){
        for(int i = 0; i<zonaAnterior.cantP; i++){
            modificarPersonaje(z.personajes[i], zonaAnterior.personajes[i]);
            if(z.personajes[i].objeto != NULL){
                modificarObjeto(z.personajes[i].objeto, obtnerIdZonaPorNombre(z.nombre));
            }
        }
        for(int i = zonaAnterior.cantP; i<z.cantP; i++){
            insertarPersonaje(z.personajes[i], z.nombre);
        }
    } else {
        for(int i = 0; i<z.cantP; i++){
            modificarPersonaje(z.personajes[i], zonaAnterior.personajes[i]);
            if(z.personajes[i].objeto != NULL){
                modificarObjeto(z.personajes[i].objeto, obtnerIdZonaPorNombre(z.nombre));
            }
        }
        for(int i = z.cantP; i<zonaAnterior.cantP; i++){
            eliminarPersonaje(zonaAnterior.personajes[i]);
        }
    }

    for(int i = 0; i<z.cantP; i++){
        modificarPersonaje(z.personajes[i], zonaAnterior.personajes[i]);
        if(z.personajes[i].objeto != NULL){
            modificarObjeto(z.personajes[i].objeto, obtnerIdZonaPorNombre(z.nombre));
        }

    }

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void modificarPersonaje(Personaje_Struct p, Personaje_Struct personajeAnterior){

    //Acertijo
    modificarAcertijo(p.acertijo,personajeAnterior.acertijo.pregunta);

    //Modificar el objeto si hay
    int idObjeto;
    int tieneObjeto = 0;
    if(p.objeto != NULL){
        idObjeto = obtnerIdObjetoPorNombre(p.objeto);
        tieneObjeto = 1;
    }

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "update Personaje set nombre=?, dialogo=?, pista=?, id_objeto=? where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, p.nombre, strlen(p.nombre), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, p.dialogo, strlen(p.dialogo), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p.pista, strlen(p.pista), SQLITE_STATIC);
    if (tieneObjeto){
        sqlite3_bind_int(stmt, 4, idObjeto);
    } else {
        sqlite3_bind_null(stmt, 4);
    }
    sqlite3_bind_text(stmt, 5, personajeAnterior.nombre, strlen(personajeAnterior.nombre), SQLITE_STATIC);
    
    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error modificar el personaje\n");
	}else{
		printf("Personaje %s modificado\n", p.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void modificarAcertijo(Acertijo_Struct a, char* preguntaAnterior){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "update Acertijo set pregunta=?, respuesta = ? where pregunta=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, a.pregunta, strlen(a.pregunta), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, a.respuesta, strlen(a.respuesta), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, preguntaAnterior, strlen(preguntaAnterior), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error modificar el acertijo\n");
	}else{
		printf("Acertijo %s modificado\n", a.pregunta);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void modificarObjeto(char* o, int idZona){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "update Objeto set nombre=? where id=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, o, strlen(o), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idZona);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error modificado el objeto\n");
	}else{
		printf("Objeto %s modificado\n", o);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}


void eliminarPueblo(Pueblo_Struct p){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "delete from Pueblo where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, p.nombre, strlen(p.nombre), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Insertar las zonas
    for(int i = 0; i<p.cantZonas; i++){
        eliminarZona(p.zonas[i]);
    }

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error eliminando el pueblo\n");
	}else{
		printf("Pueblo %s eliminado\n", p.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void eliminarZona(Zona_Struct z){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "Delete from Zona where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, z.nombre, strlen(z.nombre), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Eliminar Personaje
    for(int i = 0; i<z.cantP; i++){
        eliminarPersonaje(z.personajes[i]);
    }

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error eliminando el zona\n");
	}else{
		printf("Zona %s eliminando\n", z.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void eliminarPersonaje(Personaje_Struct p){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "Delete from Personaje where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, p.nombre, strlen(p.nombre), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    if(p.objeto){
        eliminarObjeto(p.objeto);
    }

    //Acertijo
    eliminarAcertijo(p.acertijo);
    

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error eliminando el personaje\n");
	}else{
		printf("Personaje %s eliminado\n", p.nombre);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void eliminarAcertijo(Acertijo_Struct a){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "Delete from Acertijo where pregunta=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, a.pregunta, strlen(a.pregunta), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error eliminando el acertijo\n");
	}else{
		printf("Acertijo %s eliminado\n", a.pregunta);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}

void eliminarObjeto(char* o){

    sqlite3 *db;
	sqlite3_stmt *stmt;
	int result;

    //Abrir base de datos
    sqlite3_open(obtener_valor_config("db_file"), &db);

    //Sentencia sql
    char sql[] = "Delete from Objeto  where nombre=?";

    //Preparar sentencia sql
    sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) ;
	sqlite3_bind_text(stmt, 1, o, strlen(o), SQLITE_STATIC);

    result = sqlite3_step(stmt);

    //Comprobar que la sentencia sql se ha ejecutado sin problemas
    if (result != SQLITE_DONE) {
		printf("Error eliminando el objeto\n");
	}else{
		printf("Objeto %s eliminado\n", o);
	}

    //Cerrar la setencia
    sqlite3_finalize(stmt);

    //Cerrar la base de datos
    sqlite3_close(db);

}