#include "privadoBD.h"
#include "procedimientosBD.h"
#include <stdlib.h>
#include <stdio.h>
#include "estructuras.h"

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
                  return;
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
producto* obtenerProductoIndividual(char* codigo) {
      producto productoEncontrado = {0};
      MYSQL *conn;
      MYSQL_STMT *stmt;
      MYSQL_BIND bind[1];
      MYSQL_RES *prepare_meta_result;
      producto* p = NULL;
      int error;
      
      error = conectar(&conn);
      if(!error) {
            stmt = mysql_stmt_init(conn);
            const char* query = "CALL ObtenerProductoIndividual(?)";
            if (mysql_stmt_prepare(stmt, query, strlen(query))) {
                  fprintf(stderr, "Error al preparar el procedimiento: %s\n", mysql_error(conn));
            }
    memset(bind, 0, sizeof(bind));
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char*)codigo;
    bind[0].buffer_length = strlen(codigo);

    if (mysql_stmt_bind_param(stmt, bind)) {
        fprintf(stderr, "Error al vincular los parámetros: %s\n", mysql_error(conn));
    }

    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "Error al ejecutar el procedimiento: %s\n", mysql_error(conn));
    }

    MYSQL_BIND result_bind[2];
    char nombreProducto[255];
    double precioProducto;

    memset(result_bind, 0, sizeof(result_bind));
    result_bind[0].buffer_type = MYSQL_TYPE_STRING;
    result_bind[0].buffer = nombreProducto;
    result_bind[0].buffer_length = sizeof(nombreProducto);

    result_bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
    result_bind[1].buffer = &precioProducto;

    if (mysql_stmt_bind_result(stmt, result_bind)) {
        fprintf(stderr, "Error al vincular los resultados: %s\n", mysql_error(conn));
    }

    if (mysql_stmt_fetch(stmt) == 0) {
        p = (producto*)malloc(sizeof(producto));
        if (!p) {
            fprintf(stderr, "Error al asignar memoria para producto.\n");
        }
        
        p->codigoProducto = strdup(codigo);
        p->nombreProducto = strdup(nombreProducto);
        p->cantidadProducto = 0; 
        p->precio = precioProducto;
        p->siguiente = NULL;

    } else {
        fprintf(stderr, "No se encontraron resultados.\n");
    }
    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);

    return p;
      }
      
  }
  
  
void guardarDatosCotizacion(producto* lista){
      MYSQL *conn;
      int error;
      error = conectar(&conn);
      if(!error){
            MYSQL_STMT *stmt;
            MYSQL_BIND param[1];
            int id_cotizacion;
            stmt = mysql_stmt_init(conn);
            printf("Aquí después de stmt\n");
      if (!stmt) {
            fprintf(stderr, "Error al inicializar statement\n");
            mysql_close(conn);
      }
      char* consulta = "CALL insertarCotizacion(?)";
    
      if (mysql_stmt_prepare(stmt, consulta, strlen(consulta))) {
            printf("Aquí después de prepare\n");
            fprintf(stderr, "Error al preparar statement: %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            mysql_close(conn);

      }
      
      memset(param, 0, sizeof(param));
    
      param[0].buffer_type = MYSQL_TYPE_LONG;
      param[0].buffer = &id_cotizacion;
      param[0].is_unsigned = 1;
      param[0].is_null = 0;
      
      if (mysql_stmt_bind_param(stmt, param)) {
            fprintf(stderr, "Error al bindear parámetros: %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            mysql_close(conn);

      }
    
      if (mysql_stmt_execute(stmt)) {
            fprintf(stderr, "Error al ejecutar procedimiento: %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            mysql_close(conn);

      }
      if (mysql_stmt_bind_result(stmt, param)) {
            fprintf(stderr, "Error al bindear los resultados: %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            mysql_close(conn);
            return; 
      }
      if (mysql_stmt_fetch(stmt)) {
            fprintf(stderr, "Error al obtener el valor de salida: %s\n", mysql_stmt_error(stmt));
            mysql_stmt_close(stmt);
            mysql_close(conn);
            return;
      }
    
      printf("ID de cotización generado: %d\n", id_cotizacion);

      agregarProductosCot(id_cotizacion, lista);

      mysql_stmt_close(stmt);
      mysql_close(conn);
      }

}

void agregarProductosCot(int numCotizacion, producto* lista) {
      MYSQL *conn;
      MYSQL_RES *res;
      conn = mysql_init(NULL);
      
      if (conn == NULL) {
          fprintf(stderr, "Error al inicializar la conexión: %s\n", mysql_error(conn));
          return;
      }
      
      if (mysql_real_connect(conn, HOST, USERNAME, PASSWORD, 
                           DATABASE, 0, NULL, 0) == NULL) {
          fprintf(stderr, "Error al conectar: %s\n", mysql_error(conn));
          mysql_close(conn);
          return;
      }

      producto *actual = lista;
      while (actual != NULL) {
          if (actual->codigoProducto == NULL) {
              fprintf(stderr, "Advertencia: Código de producto NULL, omitiendo...\n");
              actual = actual->siguiente;
              continue;
          }
          char query[512];
          snprintf(query, sizeof(query), 
                  "CALL insertarCotizacionProducto(%d, '%s', %d)",
                  numCotizacion, actual->codigoProducto, actual->cantidadProducto);
          if (mysql_query(conn, query)) {
              fprintf(stderr, "Error al insertar producto %s: %s\n", 
                      actual->codigoProducto, mysql_error(conn));

              actual = actual->siguiente;
              continue;
          }
          do {
              res = mysql_store_result(conn);
              if (res) {
                  mysql_free_result(res);
              }
          } while (mysql_next_result(conn) == 0);
          
          actual = actual->siguiente;
      }
      mysql_close(conn);
  }

  // Estadisticas

  void mostrarEstadisticasCotizaciones() {
      MYSQL *conexion;
      if(conectar(&conexion)) return;
  
      printf("\n=== Estadisticas de cotizaciones ===\n");
      
      // Cotizaciones pendientes
      if(mysql_query(conexion, "CALL obtener_cotizaciones_pendientes()")) {
          printf("Error: %s\n", mysql_error(conexion));
      } else {
          MYSQL_RES *resultado = mysql_store_result(conexion);
          if(resultado) {
              MYSQL_ROW fila = mysql_fetch_row(resultado);
              printf("Cotizaciones pendientes: %s\n", fila[0]);
              mysql_free_result(resultado);
          }
      }
  
      // Cotizaciones facturadas
      if(mysql_query(conexion, "CALL obtener_cotizaciones_facturadas()")) {
          printf("Error: %s\n", mysql_error(conexion));
      } else {
          MYSQL_RES *resultado = mysql_store_result(conexion);
          if(resultado) {
              MYSQL_ROW fila = mysql_fetch_row(resultado);
              printf("Cotizaciones facturadas: %s\n", fila[0]);
              mysql_free_result(resultado);
          }
      }
  
      // Promedio de compra
      if(mysql_query(conexion, "CALL obtener_promedio_compras()")) {
          printf("Error: %s\n", mysql_error(conexion));
      } else {
          MYSQL_RES *resultado = mysql_store_result(conexion);
          if(resultado) {
              MYSQL_ROW fila = mysql_fetch_row(resultado);
              printf("Promedio de compra: $%.2f\n", atof(fila[0]));
              mysql_free_result(resultado);
          }
      }
  
      mysql_close(conexion);
  }

  void mostrarTopProductos() {
      MYSQL *conexion;
      if(conectar(&conexion)) return;
  
      printf("\n=== Top 5 productos mas vendidos ===\n");
      
      if(mysql_query(conexion, "CALL obtener_top_productos()")) {
          printf("Error: %s\n", mysql_error(conexion));
      } else {
          MYSQL_RES *resultado = mysql_store_result(conexion);
          if(resultado) {
              printf("%-15s %-30s %-10s\n", "Código", "Producto", "Ventas");
              printf("--------------------------------------------\n");
              
              MYSQL_ROW fila;
              int contador = 0;
              while((fila = mysql_fetch_row(resultado)) && contador < 5) {
                  printf("%-15s %-30s %-10s\n", fila[0], fila[1], fila[2]);
                  contador++;
              }
              mysql_free_result(resultado);
          }
      }
  
      mysql_close(conexion);
  }

  void mostrarVentasPorFamilia() {
      MYSQL *conexion;
      if(conectar(&conexion)) return;
  
      printf("\n=== Ventas por familia ===\n");
      
      // Producto más vendido por familia
      printf("\nProducto más vendido por familia:\n");
      if(mysql_query(conexion, "CALL obtener_producto_popular_por_familia()")) {
          printf("Error: %s\n", mysql_error(conexion));
      } else {
          MYSQL_RES *resultado = mysql_store_result(conexion);
          if(resultado) {
              printf("%-20s %-15s %-30s %-10s\n", "Familia", "Código", "Producto", "Ventas");
              printf("------------------------------------------------------------\n");
              
              MYSQL_ROW fila;
              while((fila = mysql_fetch_row(resultado))) {
                  printf("%-20s %-15s %-30s %-10s\n", fila[0], fila[1], fila[2], fila[3]);
              }
              mysql_free_result(resultado);
          }
      }
  
      // Monto vendido por familia
      printf("\nMonto vendido por familia:\n");
      if(mysql_query(conexion, "CALL obtener_ventas_por_familia()")) {
          printf("Error: %s\n", mysql_error(conexion));
      } else {
          MYSQL_RES *resultado = mysql_store_result(conexion);
          if(resultado) {
              printf("%-20s %-15s\n", "Familia", "Total Vendido");
              printf("----------------------------------\n");
              
              MYSQL_ROW fila;
              while((fila = mysql_fetch_row(resultado))) {
                  printf("%-20s $%-15.2f\n", fila[0], atof(fila[1]));
              }
              mysql_free_result(resultado);
          }
      }
  
      mysql_close(conexion);
  }
