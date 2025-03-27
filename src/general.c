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

}

void modificarCotizacion() {
}

void facturar() {
    
}

void mostrarEstadisticas() {

}
