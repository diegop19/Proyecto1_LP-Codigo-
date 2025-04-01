#include "privadoBD.h"
#include "procedimientosBD.h"
#include <stdlib.h>
#include <stdio.h>
#include "estructuras.h"

producto* crearProducto(const char* codigo, const char* nombre, int cantidad, double precio) {
      producto* nuevo = (producto*)malloc(sizeof(producto));
      if (nuevo == NULL) {
          return NULL;
      }
      
      nuevo->codigoProducto = strdup(codigo);
      nuevo->nombreProducto = strdup(nombre);
      nuevo->cantidadProducto = cantidad;
      nuevo->precio = precio;
      nuevo->siguiente = NULL;
      
      return nuevo;
  }

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



  int eliminarProductoBD(char* identificador) {
      MYSQL *conexion;
      int resultado = 0;
      
      if(conectar(&conexion)) {
          return 0; 
      }
      
      MYSQL_STMT *stmt = mysql_stmt_init(conexion);
      if(!stmt) {
          mysql_close(conexion);
          return 0;
      }
      
      const char *consulta = "CALL removerProducto(?)";
      if(mysql_stmt_prepare(stmt, consulta, strlen(consulta))) {
          fprintf(stderr, "Error al preparar consulta: %s\n", mysql_error(conexion));
          mysql_stmt_close(stmt);
          mysql_close(conexion);
          return 0;
      }
      
      MYSQL_BIND bind[1];
      memset(bind, 0, sizeof(bind));
      
      bind[0].buffer_type = MYSQL_TYPE_STRING;
      bind[0].buffer = identificador;
      bind[0].buffer_length = strlen(identificador);
      
      if(mysql_stmt_bind_param(stmt, bind)) {
          fprintf(stderr, "Error al vincular parámetros: %s\n", mysql_error(conexion));
          mysql_stmt_close(stmt);
          mysql_close(conexion);
          return 0;
      }
      
      if(mysql_stmt_execute(stmt)) {
          unsigned int err_num = mysql_stmt_errno(stmt);
          if(err_num == 1451) { 
              printf("No se puede eliminar: producto está en uso\n");
          } else {
              fprintf(stderr, "Error al ejecutar: %s\n", mysql_stmt_error(stmt));
          }
      } else {
          if(mysql_stmt_affected_rows(stmt) > 0) {
              resultado = 1; 
          }
      }
      
      mysql_stmt_close(stmt);
      mysql_close(conexion);
      return resultado;
  }


  void mostrarResultado(const char* consulta, const char* mensaje, int esDecimal) {
    MYSQL *conexion;
    if(conectar(&conexion)) return;
    if(mysql_query(conexion, consulta)) {
        printf("Error: %s\n", mysql_error(conexion));
        return;
    }
    
    MYSQL_RES *resultado = mysql_store_result(conexion);
    if(resultado) {
        MYSQL_ROW fila = mysql_fetch_row(resultado);
        if(fila && fila[0]) {
            if(esDecimal) {
                printf("%s: $%.2f\n", mensaje, atof(fila[0]));
            } else {
                printf("%s: %s\n", mensaje, fila[0]);
            }
        } else {
            printf("%s: No se encontraron datos\n", mensaje);
        }
        mysql_free_result(resultado);
    }
    
    // Consumir cualquier resultado adicional
    while(mysql_next_result(conexion) == 0) {
        MYSQL_RES *res_extra = mysql_store_result(conexion);
        if(res_extra) mysql_free_result(res_extra);
    }
}
  void mostrarEstadisticasCotizaciones() {
    printf("\n=== Estadisticas de cotizaciones ===\n");
    mostrarResultado("CALL sp_obtener_cotizaciones_pendientes()", 
                    "Cotizaciones pendientes", 0);
    mostrarResultado("CALL sp_obtener_cotizaciones_facturadas()", 
                    "Cotizaciones facturadas", 0);
    mostrarResultado("CALL sp_obtener_promedio_compras()", 
                    "Promedio de compra", 1);
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
  producto* datosCotizacion(int numCotizacion){
      MYSQL* conexion;
      MYSQL_RES* res;
      MYSQL_ROW row;
      
      producto* lista = NULL;
      producto* ultimo = NULL;
      
      
      int error = conectar(&conexion);
      if(!error) {
            char query[256];
            snprintf(query, sizeof(query), 
                     "CALL obtenerCotizaciones(%d)", numCotizacion);
            
            if (mysql_query(conexion, query)) {
                fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conexion));
                mysql_close(conexion);
                return NULL;
            }

            res = mysql_store_result(conexion);
            if (res == NULL) {
                fprintf(stderr, "Error al obtener resultados: %s\n", mysql_error(conexion));
                mysql_close(conexion);
                return NULL;
            }
            
            while ((row = mysql_fetch_row(res)) != NULL) {
                producto* nuevo = crearProducto(
                    row[0],  
                    row[1],  
                    atoi(row[3]),  
                    atof(row[2])   
                );
                
                if (nuevo == NULL) {
                    fprintf(stderr, "Error al crear producto\n");
                    mysql_free_result(res);
                    mysql_close(conexion);
                    return NULL;
                }
                
                if (lista == NULL) {
                    lista = nuevo;
                    ultimo = nuevo;
                } else {
                    ultimo->siguiente = nuevo;
                    ultimo = nuevo;
                }
            }
            mysql_free_result(res);
            mysql_close(conexion);
            
            return lista;

      }
  }

  int actualizarCotizacionBD(int idCotizacion, producto* lista) {
      MYSQL* conexion;
      int error = 0;
      if (conectar(&conexion)) {
          fprintf(stderr, "Error al conectar a la base de datos\n");
          return 1;
      }
      producto* actual = lista;
      while (actual != NULL && !error) {
          char query[512];
          snprintf(query, sizeof(query),
                  "CALL actualizarCotizacionProducto(%d, '%s', %d)",
                  idCotizacion, 
                  actual->codigoProducto, 
                  actual->cantidadProducto);
          if (mysql_query(conexion, query)) {
              fprintf(stderr, "Error al actualizar producto %s: %s\n", 
                      actual->codigoProducto, mysql_error(conexion));
              error = 1;
          }
          
          actual = actual->siguiente;
      }
      
      mysql_close(conexion);
      return error;
  }

  void eliminarProductosLista(int idCotizacion, codigoProducto* lista) {
      MYSQL *conn;
      MYSQL_STMT *stmt;
      MYSQL_BIND params[2];
      const char *query = "CALL eliminarProductoCotizacion(?, ?)";
      int error;
      error = conectar(&conn);
      if(!error){
            stmt = mysql_stmt_init(conn);
            if (mysql_stmt_prepare(stmt, query, strlen(query))) {
                  fprintf(stderr, "Error al preparar stmt: %s\n", mysql_stmt_error(stmt));
                  mysql_stmt_close(stmt);
                  return;
            }
            memset(params, 0, sizeof(params));
            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = &idCotizacion;
            params[0].is_null = 0;
    
            params[1].buffer_type = MYSQL_TYPE_STRING;
            params[1].is_null = 0;
    
            codigoProducto *actual = lista;
            while (actual != NULL) {
            params[1].buffer = actual->codigo;
            params[1].buffer_length = strlen(actual->codigo);
            if (mysql_stmt_bind_param(stmt, params)) {
                  fprintf(stderr, "Error al vincular parámetros: %s\n", mysql_stmt_error(stmt));
                  actual = actual->siguiente;
                  continue;
            }
            if (mysql_stmt_execute(stmt)) {
                  fprintf(stderr, "Error al ejecutar procedimiento para código %s: %s\n", 
                  actual->codigo, mysql_stmt_error(stmt));
            }
            while(mysql_stmt_next_result(stmt) == 0) {
                  MYSQL_RES *res = mysql_stmt_result_metadata(stmt);
                  if (res) mysql_free_result(res);
            }
        actual = actual->siguiente;
    }
    mysql_stmt_close(stmt);

      }
}
int obtenerNumeroFactura() {
      MYSQL *conn;
      MYSQL_RES *res;
      MYSQL_ROW row;
      int numeroFactura = -1; 
      int error = conectar(&conn);
      if(!error){

            if (mysql_query(conn, "CALL CrearFactura();")) {
            fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
            return -1;  
            }
            res = mysql_store_result(conn);
            if (res == NULL) {
            fprintf(stderr, "Error al obtener el resultado: %s\n", mysql_error(conn));
            return -1;  
            }
  
            row = mysql_fetch_row(res);
            if (row != NULL) {
        
                  numeroFactura = atoi(row[0]);
            } else {
            printf("No se pudo obtener el número de la factura.\n");
            }
            mysql_free_result(res);
  
            return numeroFactura;  
      }
}

void facturarProductos(int idFactura, producto* lista) {
      MYSQL *conn;
      int error;
      error = conectar(&conn);
      
      if(!error) {
          MYSQL_STMT *stmt;
          MYSQL_BIND param[4];
          
          stmt = mysql_stmt_init(conn);
          if (!stmt) {
              fprintf(stderr, "Error al inicializar statement\n");
              mysql_close(conn);
              return;
          }
          
          char* consulta = "CALL guardarFacturaProducto(?, ?, ?, ?)";
          
          if (mysql_stmt_prepare(stmt, consulta, strlen(consulta))) {
              fprintf(stderr, "Error al preparar statement: %s\n", mysql_stmt_error(stmt));
              mysql_stmt_close(stmt);
              mysql_close(conn);
              return;
          }
          
          // Recorremos la lista de productos
          producto* actual = lista;
          while(actual != NULL) {
              memset(param, 0, sizeof(param));
              
              // Parámetro 1: idFactura (INT)
              param[0].buffer_type = MYSQL_TYPE_LONG;
              param[0].buffer = &idFactura;
              param[0].is_unsigned = 1;
              param[0].is_null = 0;
              
              // Parámetro 2: identificadorProducto (VARCHAR) - usamos codigoProducto
              param[1].buffer_type = MYSQL_TYPE_STRING;
              param[1].buffer = actual->codigoProducto;
              param[1].buffer_length = strlen(actual->codigoProducto);
              param[1].is_null = 0;
              
              // Parámetro 3: cantidad (INT) - usamos cantidadProducto
              param[2].buffer_type = MYSQL_TYPE_LONG;
              param[2].buffer = &(actual->cantidadProducto);
              param[2].is_unsigned = 1;
              param[2].is_null = 0;
              
              // Parámetro 4: precioUnitario (DOUBLE) - usamos precio
              param[3].buffer_type = MYSQL_TYPE_DOUBLE;
              param[3].buffer = &(actual->precio);
              param[3].is_null = 0;
              
              if (mysql_stmt_bind_param(stmt, param)) {
                  fprintf(stderr, "Error al bindear parámetros: %s\n", mysql_stmt_error(stmt));
                  mysql_stmt_close(stmt);
                  mysql_close(conn);
                  return;
              }
              
              if (mysql_stmt_execute(stmt)) {
                  fprintf(stderr, "Error al ejecutar procedimiento: %s\n", mysql_stmt_error(stmt));
                  mysql_stmt_close(stmt);
                  mysql_close(conn);
                  return;
              }
              
              // --- Cambio importante: Consumir todos los resultados ---
              do {
                  MYSQL_RES *result = mysql_stmt_result_metadata(stmt);
                  if (result) {
                      mysql_stmt_store_result(stmt);
                      MYSQL_ROW row;
                      while (!mysql_stmt_fetch(stmt)) {
                          // Puedes procesar el mensaje de salida si lo necesitas
                      }
                      mysql_free_result(result);
                  }
              } while (!mysql_stmt_next_result(stmt)); // Avanzar al siguiente resultado si hay
              
              actual = actual->siguiente;
          }
          
          mysql_stmt_close(stmt);
          mysql_close(conn);
      }
  }
int obtenerCantidadDisponible(const char *idProducto){
      MYSQL *conn;
      int error = conectar(&conn);
      if(!error){
            MYSQL_RES *res;
            MYSQL_ROW row;
            int cantidadEnStock = -1;

    
            char query[256];
            snprintf(query, sizeof(query), "CALL obtenerDisponible('%s', @cantidadDisponible);", idProducto);

            if (mysql_query(conn, query)) {
                  fprintf(stderr, "Error al ejecutar la consulta: %s\n", mysql_error(conn));
                  return -1; 
            }

            if (mysql_query(conn, "SELECT @cantidadDisponible;")) {
                  fprintf(stderr, "Error al obtener el resultado de la variable: %s\n", mysql_error(conn));
                  return -1;
            }

            res = mysql_store_result(conn);
            if (res == NULL) {
                  fprintf(stderr, "Error al almacenar el resultado: %s\n", mysql_error(conn));
                  return -1;
            }
            row = mysql_fetch_row(res);
            if (row != NULL) {
                  cantidadEnStock = atoi(row[0]);
            }
            mysql_free_result(res);

            return cantidadEnStock;  
      }

}
void rebajarStock(const char *codigoProducto, int cantidadRebajar){
      MYSQL *conn;
      int error = conectar(&conn);
      if(!error){
            MYSQL_STMT *stmt;
            MYSQL_BIND params[2]; 
    const char *query = "CALL reabajarStock(?, ?)";

    stmt = mysql_stmt_init(conn);
    if (!stmt) {
        fprintf(stderr, "Error al inicializar el statement: %s\n", mysql_error(conn));
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        fprintf(stderr, "Error al preparar la consulta: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    memset(params, 0, sizeof(params));


    params[0].buffer_type = MYSQL_TYPE_STRING;
    params[0].buffer = (char *)codigoProducto;
    params[0].buffer_length = strlen(codigoProducto);


    params[1].buffer_type = MYSQL_TYPE_LONG;
    params[1].buffer = &cantidadRebajar;

 
    if (mysql_stmt_bind_param(stmt, params)) {
        fprintf(stderr, "Error al asociar parámetros: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "Error al ejecutar el procedimiento: %s\n", mysql_stmt_error(stmt));
    } else {

        printf("Stock actualizado exitosamente.\n");
    }


    mysql_stmt_close(stmt);


      }

}

void actualizarFactura(int identificadorFactura, double subtotal, double total){
      MYSQL *conn;
      int error;
      error = conectar(&conn);
      if(!error){
            MYSQL_STMT *stmt;
            MYSQL_BIND params[3]; 
            const char *query = "CALL actualizarFactura(?, ?, ?)";

            stmt = mysql_stmt_init(conn);
            if (!stmt) {
                fprintf(stderr, "Error al inicializar el statement: %s\n", mysql_error(conn));
                return;
            }
            if (mysql_stmt_prepare(stmt, query, strlen(query))) {
                fprintf(stderr, "Error al preparar la consulta: %s\n", mysql_stmt_error(stmt));
                mysql_stmt_close(stmt);
                return;
             }

            memset(params, 0, sizeof(params));
            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = &identificadorFactura;

            params[1].buffer_type = MYSQL_TYPE_DOUBLE;
            params[1].buffer = &subtotal;

            params[2].buffer_type = MYSQL_TYPE_DOUBLE;
            params[2].buffer = &total;

            if (mysql_stmt_bind_param(stmt, params)) {
                fprintf(stderr, "Error al asociar los parámetros: %s\n", mysql_stmt_error(stmt));
                mysql_stmt_close(stmt);
                return;
            }
    
            // Ejecutar el statement
            if (mysql_stmt_execute(stmt)) {
                fprintf(stderr, "Error al ejecutar el procedimiento: %s\n", mysql_stmt_error(stmt));
            } else {
                printf("Factura con ID %d actualizada exitosamente.\n", identificadorFactura);
            }

            mysql_stmt_close(stmt);
        }
}
void desplegarFacturas() {

    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int error = conectar(&conn);
    if (error) {
        return; 
    }

    if (mysql_query(conn, "SELECT * FROM FACTURA")) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // Obtener los resultados de la consulta
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Error al obtener los resultados: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // Obtener el número de columnas
    int num_fields = mysql_num_fields(res);

    // Mostrar los nombres de las columnas (cabecera)
    printf("%-15s %-15s %-15s %-15s\n", "ID_FACTURA", "FECHA", "SUBTOTAL", "TOTAL");
    printf("------------------------------------------------------------\n");

    // Mostrar los resultados
    while ((row = mysql_fetch_row(res))) {
        printf("%-10s %-20s %-15s %-20s\n", 
            row[0] ? row[0] : "NULL", 
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL", 
            row[3] ? row[3] : "NULL");
    }

    // Liberar recursos
    mysql_free_result(res);
    mysql_close(conn);
}

void desplegarDetallesFactura(int factura_id) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

   
    int error = conectar(&conn);
    if (error) {
        return; 
    }

    char query[256];
    snprintf(query, sizeof(query), "CALL desplegarProductosFactura(%d);", factura_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar el procedimiento: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Error al obtener los resultados de la primera consulta: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    printf("Datos de la factura:\n");
    printf("%-15s %-20s %-15s %-15s\n", "ID_FACTURA", "FECHA", "SUBTOTAL", "TOTAL");
    printf("------------------------------------------------------------\n");
    
    while ((row = mysql_fetch_row(res))) {
        printf("%-15s %-20s %-15s %-15s\n", 
            row[0] ? row[0] : "NULL", 
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL", 
            row[3] ? row[3] : "NULL");
    }

    mysql_free_result(res);

    if (mysql_next_result(conn) != 0) {
        fprintf(stderr, "Error al obtener el siguiente conjunto de resultados: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // Procesar el segundo conjunto de resultados (productos de la factura)
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Error al obtener los resultados de la segunda consulta: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    // Mostrar los resultados de la segunda consulta (productos)
    printf("\nProductos de la factura:\n");
    printf("%-15s %-30s %-10s %-15s\n", "ID_PRODUCTO", "NOMBRE", "CANTIDAD", "PRECIO_UNITARIO");
    printf("------------------------------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%-15s %-30s %-10s %-15s\n", 
            row[0] ? row[0] : "NULL", 
            row[1] ? row[1] : "NULL", 
            row[2] ? row[2] : "NULL", 
            row[3] ? row[3] : "NULL");
    }

    mysql_free_result(res);

    // Asegurarse de que no hay más conjuntos de resultados
    while (mysql_next_result(conn) == 0) {
        res = mysql_store_result(conn);
        mysql_free_result(res);
    }

    // Cerrar la conexión
    mysql_close(conn);
}


int actualizarEstadoCotizacionBD(int numCotizacion) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int resultado = -2;

    // Conectar a la base de datos (asumiendo que tienes una función conectar() definida)
    if (conectar(&conn) != 0) {
        fprintf(stderr, "Error al conectar a la base de datos\n");
        return resultado;
    }

    // Preparar la llamada al procedimiento almacenado
    char query[128];
    snprintf(query, sizeof(query), "CALL actualizarEstadoCotizacion(%d, @resultado);", numCotizacion);

    // Ejecutar el procedimiento
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al llamar al procedimiento: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -2;
    }

    // Obtener el resultado
    if (mysql_query(conn, "SELECT @resultado;")) {
        fprintf(stderr, "Error al obtener el resultado: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -2;
    }

    res = mysql_store_result(conn);
    if (res && (row = mysql_fetch_row(res))) {
        resultado = atoi(row[0]); // Convertir el string a entero
    }

    // Liberar recursos
    if (res) mysql_free_result(res);
    mysql_close(conn);

    return resultado;
}


int agregarClienteBD(int numCedula, const char *nombre, const char *apellido){
    MYSQL *conn;
    MYSQL_ROW row;

    if (conectar(&conn) != 0) {
        printf("No hay conexion\n");
        return -1;
}
char query[512];
    MYSQL_RES *res;
    int resultado = 0;
    

    snprintf(query, sizeof(query), 
             "CALL agregarCliente(%d, '%s', '%s', @resultado, @mensaje)", 
             numCedula, nombre, apellido);
    

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al llamar al procedimiento: %s\n", mysql_error(conn));
        return -1;
    }
    

    do {
        res = mysql_store_result(conn);
        if (res) {
            mysql_free_result(res);
        }
    } while (mysql_next_result(conn) == 0);
    
    if (mysql_query(conn, "SELECT @resultado, @mensaje")) {
        fprintf(stderr, "Error al obtener resultado: %s\n", mysql_error(conn));
        return -1;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row) {
            resultado = atoi(row[0]);
            printf("Mensaje: %s\n", row[1] ? row[1] : "Sin mensaje");
        }
        mysql_free_result(res);
    }
    
    return resultado;
}

int agregarFacturaACliente(int idCliente, int idFactura){
    MYSQL *conn;
    if (conectar(&conn) != 0) {
        printf("No hay conexion\n");
        return -1;
    }
    char query[512];
    MYSQL_RES *res;
    int resultado = 0;
    snprintf(query, sizeof(query), 
             "CALL agregarFacturaCliente(%d, %d, @resultado, @mensaje)", 
             idCliente, idFactura);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al llamar al procedimiento: %s\n", mysql_error(conn));
        return -1;
    }

    do {
        res = mysql_store_result(conn);
        if (res) mysql_free_result(res);
    } while (mysql_next_result(conn) == 0);

    if (mysql_query(conn, "SELECT @resultado, @mensaje")) {
        fprintf(stderr, "Error al obtener resultado: %s\n", mysql_error(conn));
        return -1;
    }
    
    res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row) {
            resultado = atoi(row[0]);
            printf("%s\n", row[1] ? row[1] : "");
        }
        mysql_free_result(res);
    }
    
    return resultado;
}

int eliminarFactura(int idFactura){
    MYSQL *conn;
    if (conectar(&conn) != 0) {
        printf("No hay conexion\n");
        return -1;
    }
    char query[128];
    snprintf(query, sizeof(query), "CALL eliminarFacturaSimple(%d)", idFactura);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        return 0;
    }
    return 1;
}
int validarCredenciales(char* nombreUsuario, char* clave){
    MYSQL *conn;
    if (conectar(&conn) != 0) {
        printf("No hay conexion\n");
        return -1;
    }else{
        char query[512];
        MYSQL_RES *res;
        int resultado = 0;
    

        snprintf(query, sizeof(query), 
             "CALL validarCredencialesAdmin('%s', '%s', @resultado)", 
             nombreUsuario, clave);
    
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error al validar credenciales: %s\n", mysql_error(conn));
            return 0;
        }

        do {
            res = mysql_store_result(conn);
            if (res) mysql_free_result(res);
        } while (mysql_next_result(conn) == 0);
        if (mysql_query(conn, "SELECT @resultado")) {
            fprintf(stderr, "Error al obtener resultado: %s\n", mysql_error(conn));
            return 0;
        }
    
        res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                resultado = atoi(row[0]);
            }
            mysql_free_result(res);
        }
    
        return resultado;
    }
}