#include "listas.h"

void verificarExistencia(char* codigo, int cantidad, producto** lista){
    producto* nuevoACotizar = obtenerProductoIndividual(codigo);
    producto* Aux = *lista;
    int encontrado = 0;
    while(Aux != NULL){
        if(strcmp(Aux->codigoProducto, codigo) == 0){
            Aux->cantidadProducto += cantidad;
            encontrado = 1;
            printf("Cantidad sumada a la cotizacion\n");
            break;
        }else{
            Aux = (Aux->siguiente);
        }
    }
    if(encontrado == 0){
        nuevoACotizar->cantidadProducto = cantidad;
        insertarProductoCotizacion(nuevoACotizar, lista);
    }

}
void insertarProductoCotizacion(producto* nuevoNodo, producto** lista){
    nuevoNodo->siguiente = NULL;
    if (*lista == NULL) {
        *lista = nuevoNodo;
    } else {
        producto* Aux = *lista;
        while (Aux->siguiente != NULL) {
            Aux = Aux->siguiente;
        }
        Aux->siguiente = nuevoNodo;
    }
    printf("Producto insertado con éxito a la cotizacion\n");
}