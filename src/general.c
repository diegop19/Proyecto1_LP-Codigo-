#include <stdio.h>
#include "general.h"

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
        }
    } while (opcion != 6);
}

void consultarCatalogo() {
  
}

void cotizar() {

}

void modificarCotizacion() {
}

void facturar() {
    
}

void mostrarEstadisticas() {

}