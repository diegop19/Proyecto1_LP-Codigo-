#include "privadoBD.h"
#include "procedimientosBD.h"
#include <stdlib.h>
#include <stdio.h>

int conectar(MYSQL **conexion){
    int error;
    *conexion = mysql_init(NULL);
    if(mysql_real_connect(*conexion, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL,0) != NULL){
          error = 0;
    }else{
          printf("Error al conectarse con la base de datos\n");
          error = 1;
    }
    return error;
}
void insertarFamiliaBD(char* idFamilia, char* descripcion){
      MYSQL *conexion;
      int error;
      error = conectar(&conexion);
      MYSQL_BIND bind[2];
      if(!error){
            MYSQL_STMT *stmt;
            const char* consulta = "CALL insertarFamilia(?,?)";
            stmt = mysql_stmt_init(conexion);
            if(mysql_stmt_prepare(stmt, consulta, strlen(consulta))){
                  printf("Error al preparar la consulta: %s\n", mysql_stmt_error(stmt));
            }
            
            memset(bind, 0, sizeof(bind));
            bind[0].buffer_type = MYSQL_TYPE_STRING;
            bind[0].buffer = idFamilia;
            bind[0].buffer_length = sizeof(idFamilia);

            bind[1].buffer_type = MYSQL_TYPE_STRING;
            bind[1].buffer = descripcion;
            bind[1].buffer_length = sizeof(descripcion);
            
            if(mysql_stmt_bind_param(stmt, bind) != 0){
                  printf("Error al vincular parámetros: %s\n", mysql_stmt_error(stmt));
            }
            if(mysql_stmt_execute(stmt) != 0){
                  printf("Error al ejecutar el procedimiento: %s\n", mysql_stmt_error(stmt));
      
            }else{
                  printf("Producto agregado exitosamente\n");
                  
            }
            mysql_stmt_close(stmt);
            mysql_close(conexion);
      }
      


}

void insertarProductoBD(char *id, char *nombre, char *familia, double costo, double precio, int cantidad_stock){
      printf("-----------------------------------------------------------\n");
      printf("Insertando producto: ID = %s, Nombre = %s, Familia = %s, Costo = %.2f, Precio = %.2f, Stock = %d\n",
            id, nombre, familia, costo, precio, cantidad_stock);
      MYSQL *conexion;
      int error;
      error = conectar(&conexion);
      MYSQL_BIND bind[6];
      if(!error){
            MYSQL_STMT *stmt;
            const char* consulta = "CALL insertar_producto(?,?,?,?,?,?)";
            stmt = mysql_stmt_init(conexion);

            if(mysql_stmt_prepare(stmt, consulta, strlen(consulta))){
                  printf("Error al preparar la consulta: %s\n", mysql_stmt_error(stmt));
            }
            memset(bind, 0, sizeof(bind));
            
            bind[0].buffer_type = MYSQL_TYPE_STRING;
            bind[0].buffer = id;
            bind[0].buffer_length = sizeof(id);

            bind[1].buffer_type = MYSQL_TYPE_STRING;
            bind[1].buffer = familia;
            bind[1].buffer_length = sizeof(familia);

            bind[2].buffer_type = MYSQL_TYPE_STRING;
            bind[2].buffer = nombre;
            bind[2].buffer_length = sizeof(nombre);

            bind[3].buffer_type = MYSQL_TYPE_DOUBLE;
            bind[3].buffer = &costo;
    
            bind[4].buffer_type = MYSQL_TYPE_DOUBLE;
            bind[4].buffer = &precio;
    
            bind[5].buffer_type = MYSQL_TYPE_LONG;
            bind[5].buffer = &cantidad_stock;
            

            if(mysql_stmt_bind_param(stmt, bind) != 0){
                  printf("Error al vincular parámetros: %s\n", mysql_stmt_error(stmt));
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
            if(mysql_stmt_execute(stmt) != 0){
                  printf("Error al ejecutar el procedimiento: %s\n", mysql_stmt_error(stmt));
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);

            }else{
                  printf("Producto agregado exitosamente\n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
            

        }

}