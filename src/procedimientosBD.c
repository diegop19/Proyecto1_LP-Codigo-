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

void actualizarStockBD(char* id, int cantidad){
      MYSQL *conexion;
      int error;
      error = conectar(&conexion);
      MYSQL_BIND bind[2];
      if(!error){
            MYSQL_STMT *stmt;
            const char* consulta = "CALL actualizarCantidadProducto(?,?)";
            
            stmt = mysql_stmt_init(conexion);
            if(mysql_stmt_prepare(stmt, consulta, strlen(consulta))){
                  printf("Error al preparar la consulta: %s\n", mysql_stmt_error(stmt));
            }
            memset(bind, 0, sizeof(bind));
            
            bind[0].buffer_type = MYSQL_TYPE_STRING;
            bind[0].buffer = id;
            bind[0].buffer_length = strlen(id);

            bind[1].buffer_type = MYSQL_TYPE_LONG;
            bind[1].buffer = &cantidad;

            if(mysql_stmt_bind_param(stmt, bind) != 0){
                  printf("Error al vincular parámetros: %s\n", mysql_stmt_error(stmt));
                  mysql_stmt_close(stmt);
                  mysql_close(conexion);
            }
            if(mysql_stmt_execute(stmt) != 0){
                  unsigned int err_num = mysql_stmt_errno(stmt);
                  const char* err_msg = mysql_stmt_error(stmt);
            
                  if(err_num == 1642) {  // Código para warnings (1642 = ER_SIGNAL_WARN)
                        printf("ADVERTENCIA: %s\n", err_msg);
                  } else {
                        printf("ERROR: %s\n", err_msg);
                  }
            }else{
                  printf("Stock actualizado correctamente.\n");
                  const char* warning_msg = mysql_error(conexion);
                  if(warning_msg && *warning_msg) {
                      printf("ADVERTENCIA: %s\n", warning_msg);
                  }
            mysql_stmt_close(stmt);
            mysql_close(conexion);

            }

      }
}

void desplegarProductos(){
      MYSQL *conexion;
      MYSQL_RES *resultado;
      MYSQL_ROW fila;
      int error;
      error = conectar(&conexion);
      if(!error){
            if (mysql_query(conexion, "CALL obtenerProductos()")) {
                  fprintf(stderr, "Error al llamar al procedimiento: %s\n", mysql_error(conexion));
                  mysql_close(conexion);
                  return;
            }
            resultado = mysql_store_result(conexion);
            MYSQL_FIELD *campos;
            unsigned int num_campos = mysql_num_fields(resultado);
            campos = mysql_fetch_fields(resultado);
            printf("\n=== LISTADO COMPLETO DE PRODUCTOS ===\n");
            for (unsigned int i = 0; i < num_campos; i++) {
                printf("%-35s", campos[i].name);
            }
            printf("\n");
            while ((fila = mysql_fetch_row(resultado))) {
                  for (unsigned int i = 0; i < num_campos; i++) {
                      printf("%-35s", fila[i] ? fila[i] : "NULL");
                  }
                  printf("\n");
            }
            mysql_free_result(resultado);
            while (mysql_next_result(conexion) == 0) {
                  resultado = mysql_store_result(conexion);
                  mysql_free_result(resultado);
            }
            mysql_close(conexion);

      }
}
void filtrarBusqueda(char* identificador) {
      MYSQL *conexion;
      int error;
      error = conectar(&conexion);
      if(!error){
            char query[256];
            snprintf(query, sizeof(query), "CALL filtrarProductos('%s');", identificador);
            if (mysql_query(conexion, query)) {
                  fprintf(stderr, "Error al ejecutar la consulta: %s\n", mysql_error(conexion));
                  mysql_close(conexion);
                  exit(1);
            }
            MYSQL_RES *res = mysql_store_result(conexion);
            if (res == NULL) {
                  fprintf(stderr, "Error al obtener los resultados: %s\n", mysql_error(conexion));
                  mysql_close(conexion);
                  exit(1);
            }
            MYSQL_FIELD *campos;
            unsigned int num_campos = mysql_num_fields(res);
            campos = mysql_fetch_fields(res);
            for (unsigned int i = 0; i < num_campos; i++) {
                  printf("%-35s", campos[i].name); 
            }
            printf("\n");
            for (unsigned int i = 0; i < num_campos; i++) {
                  printf("%-35s", "--------------------");
            }
            printf("\n");
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL) {
                  for (unsigned int i = 0; i < num_campos; i++) {
                        printf("%-35s", row[i] ? row[i] : "NULL");
                  }
                  printf("\n");
            }
            mysql_free_result(res);
            mysql_close(conexion);
      }
  }