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
void insertarFamilia(char* idFamilia, char* descripcion){
      MYSQL *conexion;
      int error;
      error = conectar(&conexion);
      MYSQL_BIND bind[2];
      if(!error){
            MYSQL_STMT *stmt;
            const char* consulta = "CALL insertar_producto(?,?,?,?,?,?)";
            stmt = mysql_stmt_init(conexion);
            memset(bind, 0, sizeof(bind));
            bind[0].buffer_type = MYSQL_TYPE_STRING;
            bind[0].buffer = idFamilia;
            bind[0].buffer_length = sizeof(idFamilia);

            bind[1].buffer_type = MYSQL_TYPE_STRING;
            bind[1].buffer = descripcion;
            bind[1].buffer_length = sizeof(descripcion);
            
            if(mysql_stmt_bind_param(stmt, bind) != 0){
                  printf("Ocurrio un error \n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
            if(mysql_stmt_execute(stmt) != 0){
                  printf("Ocurrio un error al ejecutar el procedimiento \n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
      
            }else{
                  printf("Producto agregado exitosamente\n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
      }
      


}

void insertarProducto(char *id, char *nombre, char *familia, float costo, float precio, int cantidad_stock){
      MYSQL *conexion;
      int error;
      error = conectar(&conexion);
      MYSQL_BIND bind[6];
      if(!error){
            MYSQL_STMT *stmt;
            const char* consulta = "CALL insertar_producto(?,?,?,?,?,?)";
            stmt = mysql_stmt_init(conexion);
            memset(bind, 0, sizeof(bind));
            bind[0].buffer_type = MYSQL_TYPE_STRING;
            bind[0].buffer = id;
            bind[0].buffer_length = sizeof(id);

            bind[1].buffer_type = MYSQL_TYPE_STRING;
            bind[1].buffer = nombre;
            bind[1].buffer_length = sizeof(nombre);

            bind[2].buffer_type = MYSQL_TYPE_STRING;
            bind[2].buffer = familia;
            bind[2].buffer_length = sizeof(familia);

            bind[3].buffer_type = MYSQL_TYPE_DOUBLE;
            bind[3].buffer = (char *)&costo;
            bind[3].buffer_length = sizeof(costo);

            bind[4].buffer_type = MYSQL_TYPE_DOUBLE;
            bind[4].buffer = (char *)&precio;
            bind[4].buffer_length = sizeof(precio);

            bind[5].buffer_type = MYSQL_TYPE_LONG;
            bind[5].buffer = (char *)cantidad_stock;
            bind[5].buffer_length = sizeof(cantidad_stock);

            if(mysql_stmt_bind_param(stmt, bind) != 0){
                  printf("Ocurrio un error \n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
            if(mysql_stmt_execute(stmt) != 0){
                  printf("Ocurrio un error al ejecutar el procedimiento \n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);

            }else{
                  printf("Producto agregado exitosamente\n");
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
            

        }

}