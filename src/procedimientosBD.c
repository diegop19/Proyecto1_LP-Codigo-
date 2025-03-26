#include <stdlib.h>
#include "privadoBD.h"
#include "procedimientosBD.h"
int conectar(MYSQL **conexion){
    int error;
    *conexion = mysql_init(NULL);
    if(mysql_real_connect(*conexion, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL,0) != NULL){
          printf("Se establecio la conexion a la base de datos \n");
          error = 0;
    }else{
          printf("Error al conectarse con la base de datos\n");
          error = 1;
    }
    return error;
}

void insertarProducto(char *id, char *nombre, char *familia, float costo, float precio, int cantidad_stock){


}