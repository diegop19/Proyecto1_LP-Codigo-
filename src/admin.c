#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <commdlg.h>
#include "admin.h"
#include "procedimientosBD.h"

void menuAdministrativo() {
    int opcion;
    do {
        printf("=== Menú Administrativo ===\n");
        printf("1. Registro de familias de productos\n");
        printf("2. Registro de productos\n");
        printf("3. Cargar inventario\n");
        printf("4. Consulta de facturas\n");
        printf("5. Mostrar Estadisticas\n");
        printf("6. Volver al Menú Principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrarFamiliaProductos();
                break;
            case 2:
                registroDeProductosMenu();
                break;
            case 3:
                cargarInventario();
                break;
            case 4:
                consultarFacturas();
                break;
            case 5:
                menuEstadisticas();
                break;
            case 6:
                printf("Volviendo al Menú Principal\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 5);
}

void registroDeProductosMenu() {
    int opcion;
    do {
        printf("=== Submenú de Registro de Productos ===\n");
        printf("1. Agregar productos\n");
        printf("2. Eliminar productos\n");
        printf("3. Volver al Menú Administrativo\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrarProductos();
                break;
            case 2:
                eliminarProducto();
                break;
            case 3:
                printf("Volviendo al Menú Administrativo\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

// Función para abrir el explorador de archivos y seleccionar un archivo
char* abrirExploradorArchivos() {
    OPENFILENAME ofn;           
    char rutaArchivo[256] = ""; 

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = rutaArchivo;
    ofn.nMaxFile = sizeof(rutaArchivo);
    ofn.lpstrFilter = "Archivos de texto\0*.txt\0Todos los archivos\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return strdup(rutaArchivo);  
    } else {
        return NULL;
    }
}




void registrarFamiliaProductos() {
    char *rutaArchivo;  
    FILE *archivo;      
    char linea[256];    
    rutaArchivo = abrirExploradorArchivos();
    if (rutaArchivo == NULL) {
        printf("No se seleccionó ningún archivo.\n");
        return;
    }

    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        free(rutaArchivo); 
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;

        char *id = NULL;
        char *descripcion = NULL;

        // Procesar la línea: separar el ID y la descripción
        char *token = strtok(linea, ",");  
        if (token != NULL) {
            id = strdup(token);  
            token = strtok(NULL, ",");  
            if (token != NULL) {
                descripcion = strdup(token);
                insertarFamiliaBD(id, descripcion);
            } else {
                printf("Error: Formato incorrecto en la línea: %s\n", linea);
            }
        } else {
            printf("Error: Formato incorrecto en la línea: %s\n", linea);
        }

        if (id != NULL) free(id);
        if (descripcion != NULL) free(descripcion);
    }
    fclose(archivo);
    free(rutaArchivo);  
    printf("Proceso de registro de familias completado.\n");
}

// auxiliar para la base de datos    HACE FALTA VERIFICAR QUE LA FAMILIA EXISTA
void insertarProducto(char *id, char *nombre, char *familia, float costo, float precio, int cantidad_stock) {
    printf("Insertando producto: ID = %s, Nombre = %s, Familia = %s, Costo = %.2f, Precio = %.2f, Stock = %d\n",
           id, nombre, familia, costo, precio, cantidad_stock);
    
}

void registrarProductos() {
    char *rutaArchivo; 
    FILE *archivo;      
    char linea[256];    

    rutaArchivo = abrirExploradorArchivos();
    if (rutaArchivo == NULL) {
        printf("No se seleccionó ningún archivo.\n");
        return;
    }

    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        free(rutaArchivo);  
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;

        char *id = NULL;
        char *nombre = NULL;
        char *familia = NULL;
        double costo = 0.0;
        double precio = 0.0;
        int cantidad_stock = 0;

        // Procesar la línea: separar los campos
        char *token = strtok(linea, ",");  
        if (token != NULL) {
            id = strdup(token);  
            token = strtok(NULL, ","); 
            if (token != NULL) {
                nombre = strdup(token);  
                token = strtok(NULL, ",");  
                if (token != NULL) {
                    familia = strdup(token);  
                    token = strtok(NULL, ",");  
                    if (token != NULL) {
                        costo = atof(token);  
                        token = strtok(NULL, ",");  
                        if (token != NULL) {
                            precio = atof(token); 
                            token = strtok(NULL, ","); 
                            if (token != NULL) {
                                cantidad_stock = atoi(token);  

                                insertarProductoBD(id, nombre, familia, costo, precio, cantidad_stock);
                            }
                        }
                    }
                }
            }
        }

        if (id != NULL) free(id);
        if (nombre != NULL) free(nombre);
        if (familia != NULL) free(familia);
    }
 
    fclose(archivo);
    free(rutaArchivo);  
    printf("Proceso de registro de productos completado.\n");
}


void eliminarProducto(){

}

void cargarInventario() {
    char *rutaArchivo;  
    FILE *archivo;      
    char linea[256];    
    rutaArchivo = abrirExploradorArchivos();
    if (rutaArchivo == NULL) {
        printf("No se seleccionó ningún archivo.\n");
        return;
    }
    archivo = fopen(rutaArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        free(rutaArchivo);  
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;

        char *id = NULL;
        int cantidad = 0;

        char *token = strtok(linea, ",");  
        if (token != NULL) {
            id = strdup(token);  
            token = strtok(NULL, ",");  
            if (token != NULL) {
                cantidad = atoi(token); 
                actualizarStockBD(id, cantidad);
            } else {
                printf("Error: Formato incorrecto en la línea: %s\n", linea);
            }
        } else {
            printf("Error: Formato incorrecto en la línea: %s\n", linea);
        }

        if (id != NULL) free(id);
    }

    fclose(archivo);
    free(rutaArchivo);  
    printf("Proceso de carga de inventario completado.\n");
}

void consultarFacturas() {


}

// Area de estadisticas 

void menuEstadisticas() {
    int opcion;
    do {
        printf("\n=== MENÚ DE ESTADÍSTICAS ===\n");
        printf("1. Estadísticas de cotizaciones\n");
        printf("2. Top productos más vendidos\n");
        printf("3. Ventas por familia\n");
        printf("4. Volver al menú administrativo\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch(opcion) {
            case 1:
                mostrarEstadisticasCotizaciones();
                break;
            case 2:
                mostrarTopProductos();
                break;
            case 3:
                mostrarVentasPorFamilia();
                break;
            case 4:
                printf("Volviendo al menú administrativo...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while(opcion != 4);
}
