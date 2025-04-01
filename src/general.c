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
        printf("5. Volver al Menú Principal\n");
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
                printf("Volviendo al Menú Principal...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 5);
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
        printf("1. Ver Productos\n2. Agregar Producto a la cotizacion\n3. Remover Producto\n4. Ver productos en la cotizacion\n5. Guardar y salir\n6. Salir sin guardar\nElija una opcion: ");
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
                if (listaCotizacion == NULL){
                    printf("No podemos hacer una cotizacion vacia...\n");
                    sleep(2);
                    break;
                }else{
                    guardarDatosCotizacion(listaCotizacion);
                    menuGeneral();
                    break;
                }
                
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
    int numCotizacion;
    printf("inserte el numero de cotizacion para facturar: ");
    scanf("%i", &numCotizacion);
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    if(actualizarEstadoCotizacionBD(numCotizacion) == 0){
        producto* listaElementos = datosCotizacion(numCotizacion);
        if(listaElementos != NULL){
            int numFactura = obtenerNumeroFactura();
            verificarStock(listaElementos);
            double subtotal = calcularTotal(listaElementos);
            double total = subtotal*1.13;
            actualizarFactura(numFactura, subtotal, total);
            facturarProductos(numFactura, listaElementos);
        }
    }else{
        printf("Ya se ha facturado esta cotizacion...\n");
    }
}

void mostrarEstadisticas() {

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
void modificarCotizacion(){
    char c;
    int numCotizacion;
    printf("Ingrese el numero de cotizacion\n");
    scanf("%i",&numCotizacion);
    producto* lista = datosCotizacion(numCotizacion);
    codigoProducto* listaCodigos = NULL;
    if(lista != NULL){
        int decision;
        int* cantidad;
        do{
            printf("=======MENU EDITAR COTIZACION=======\n");
            printf("1.Reducir o aumentar cantidad\n2.Eliminar un producto\n3.Agregar un producto\n4.Ver mis productos\n5.Guardar cambios\n6.Salir\n7.Ver productos\nIngrese una opción:\n");
            scanf("%i", &decision);
            
            switch(decision){
                case 1:
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    printf("Indique el codigo del articulo que desea editar: ");
                    char* codigoProducto = obtenerInput();
                    printf("\nIndique la cantidad a sumar o restar(si suma coloque el numero normalmente, si resta coloque un menos (-) al frente)\n");
                    int cantidad;
                    scanf("%i", &cantidad);
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    reducirAumentarCantidades(cantidad, codigoProducto, lista);
                    break;
                case 2:
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    printf("Ingrese el codigo del producto que desea eliminar: ");
                    char* codigo = obtenerInput();
                    printf("\n");
                    agregarCodigoEliminar(&listaCodigos, codigo);
                    removerProductoCot(codigo, &lista);
                    break;
                case 3:
                    agregarProductoCotizacion(&lista);
                    break;
                case 4:
                    verProductosEnCotizacion(&lista);
                    break;
                case 5:
                    guardarCambios(numCotizacion, lista);
                    eliminarProductosLista(numCotizacion, listaCodigos);
                    break;
                case 6:
                    break;
                case 7:
                    consultarCatalogo();
                    break;
                default:
                    printf("Ingrese una opcion valida\n");
                    break;
            }
        }while(decision != 6);
    }else{
        printf("Esa cotizacion no existe\n");
    }
}
void guardarCambios(int idCotizacion, producto* lista) {
    printf("\nGuardando cambios en la base de datos...\n");
    
    if (actualizarCotizacionBD(idCotizacion, lista) == 0) {
        printf("¡Cotización actualizada exitosamente!\n");
    } else {
        printf("Ocurrieron errores al actualizar algunos productos.\n");
    }
}