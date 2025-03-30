#include "listas.h"

void verificarExistencia(char* codigo, int cantidad, producto** lista){
    producto* nuevoACotizar = obtenerProductoIndividual(codigo);
    if(nuevoACotizar == NULL){
        
    }else{
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
void removerProductoCot(char* codigo, producto** lista) {
    if (*lista == NULL) {
        printf("La lista está vacía.\n");
        return;
    }

    producto* actual = *lista;
    producto* anterior = NULL;
    while (actual != NULL) {
        if (strcmp(actual->codigoProducto, codigo) == 0) {
            if (anterior == NULL) {
                *lista = actual->siguiente;  
            } else {
                anterior->siguiente = actual->siguiente;  
            }
            free(actual->codigoProducto);
            free(actual->nombreProducto);
            free(actual);
            printf("Producto con código %s eliminado.\n", codigo);
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    printf("Producto con código %s no encontrado.\n", codigo);
}