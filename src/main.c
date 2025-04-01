#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "admin.h"
#include "general.h"


void mostrarMenuPrincipal() {
    printf("=== Menú Principal ===\n");
    printf("1. Opciones Administrativas\n");
    printf("2. Opciones Generales\n");
    printf("3. Salir\n");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int opcion;
    do {
        mostrarMenuPrincipal();
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                char c;
                while ((c = getchar()) != '\n' && c != EOF) {}
                char* nombreUsuario;
                printf("Inserte el nombre de usuario: ");
                nombreUsuario = obtenerInput();
                printf("\nIngrese la contraseña: ");
                char* contraseña = obtenerInput();
                verificarCredenciales(nombreUsuario, contraseña);
                break;
            case 2:
                menuGeneral();
                break;
            case 3:
                printf("Saliendo\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}

