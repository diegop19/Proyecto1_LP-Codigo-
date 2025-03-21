#include <stdio.h>
#include "admin.h"

void menuAdministrativo() {
    int opcion;
    do {
        printf("=== Menú Administrativo ===\n");
        printf("1. Registro de familias de productos\n");
        printf("2. Registro de productos\n");
        printf("3. Cargar inventario\n");
        printf("4. Consulta de facturas\n");
        printf("5. Volver al Menú Principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrarFamiliaProductos();
                break;
            case 2:
                registrarProducto();
                break;
            case 3:
                cargarInventario();
                break;
            case 4:
                consultarFacturas();
                break;
            case 5:
                printf("Volviendo al Menú Principal\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 5);
}

void registrarFamiliaProductos() {
   
}

void registrarProducto() {
}

void cargarInventario() {

  
}

void consultarFacturas() {


}