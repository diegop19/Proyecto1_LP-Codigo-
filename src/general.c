#include <stdio.h>
#include <stdlib.h>
#include "general.h"
#include "procedimientosBD.h"

#include <string.h>
#include <unistd.h>



char* obtenerInput() {
    char c;
    int longitud = 0;
    int tamanio = 1;
    char* strUsuario = malloc(1);
    while ((c = getchar()) != '\n' && c != EOF) {
        strUsuario[longitud] = c;
        longitud++;
        if (longitud + 1 >= tamanio) {
            tamanio *= 2;
            strUsuario = realloc(strUsuario, tamanio);
        }
    }
    strUsuario[longitud] = '\0';
    return strUsuario;
}

void menuGeneral() {
    int opcion;
    do {
        printf("=== Menú General ===\n");
        printf("1. Consulta de catálogo\n");
        printf("2. Cotizar\n");
        printf("3. Modificar cotización\n");
        printf("4. Facturar\n");
        printf("5. Estadísticas\n");
        printf("6. Volver al Menú Principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                consultarCatalogo();
                break;
            case 2:

                cotizar();
                break;
            case 3:
                modificarCotizacion();
                break;
            case 4:
                facturar();
                break;
            case 5:
                mostrarEstadisticas();
                break;
            case 6:
                printf("Volviendo al Menú Principal...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 6);
}

void consultarCatalogo() {
    desplegarProductos();
    printf("¿Quiere filtrar la busqueda?  Y/N \n");
    char* respuesta;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    respuesta = obtenerInput();
    if(strcmp(respuesta, "Y") == 0 || strcmp(respuesta, "y") == 0){
        printf("Ingrese el identificador de la familia que quiere filtrar\n");
        char* identificador;
        identificador = obtenerInput();
        filtrarBusqueda(identificador);
        free(identificador);
    }
    free(respuesta);
}

void cotizar() {
    int opcion;
    producto* listaCotizacion = NULL;
    do{
        printf("=====MENU COTIZACION=====\n");
        printf("1. Ver Productos\n2. Agregar Producto a la cotizacion\n3. Remover Producto\n4. Ver productos en la cotizacion\n5. Guardar\n6. Salir sin guardar\n");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                consultarCatalogo();
                break;
            case 2:
                agregarProductoCotizacion(&listaCotizacion);
                break;
            case 3: 
                removerProductoCotizacion(&listaCotizacion);
                break;
            case 4:
                verProductosEnCotizacion(&listaCotizacion);
                break;
            case 5:
                guardarDatosCotizacion();
                break;
            case 6:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Seleccione una opcion...\n");
                break;
        }

    }while(opcion != 6);
}
void agregarProductoCotizacion(producto** listaCotizacion){
    char* codigoProducto;
    int cantidad;
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    printf("Inserte el codigo del producto a cotizar: ");
    codigoProducto = obtenerInput();
    printf("\nInserte la cantidad de productos que desea agregar: ");
    scanf("%i", &cantidad);
    printf("\n");
    verificarExistencia(codigoProducto, cantidad, listaCotizacion);
    
}

void modificarCotizacion() {
}
void removerProductoCotizacion(producto** lista){
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    printf("Inserte el codigo del producto que desea remover: ");
    char* codigo = obtenerInput();
    printf("Removiendo...\n");
    sleep(2);
    removerProductoCot(codigo, lista);
    free(codigo);
}

void facturar() {
    
}

void mostrarEstadisticas() {

}
void guardarDatosCotizacion(){

}
void verProductosEnCotizacion(producto **lista){
    if (*lista == NULL) {
        printf("No hay productos en la cotización.\n");
        return;
    }

    printf("===== Productos en Cotización =====\n");
    producto* actual = *lista;
    
    while (actual != NULL) {
        printf("-----------------------------------\n");
        printf("Código: %s\n", actual->codigoProducto);
        printf("Nombre: %s\n", actual->nombreProducto);
        printf("Cantidad: %d\n", actual->cantidadProducto);
        printf("Precio: %.2f\n", actual->precio);
        printf("Subtotal: %.2f\n", actual->cantidadProducto * actual->precio);
        printf("-----------------------------------\n");
        actual = actual->siguiente;
    }

    printf("===================================\n");
}
