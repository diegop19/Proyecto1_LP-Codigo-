#include "listas.h"

/**
 * verificarExistencia
 * Entrada: codigo (string), cantidad (entero), lista (doble puntero a producto)
 * Salida: Ninguna
 * Descripción: Verifica si un producto existe y lo agrega o actualiza en la lista de cotización
 */
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

/**
 * insertarProductoCotizacion
 * Entrada: nuevoNodo (puntero a producto), lista (doble puntero a producto)
 * Salida: Ninguna
 * Descripción: Inserta un nuevo producto al final de la lista de cotización
 */
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

/**
 * removerProductoCot
 * Entrada: codigo (string), lista (doble puntero a producto)
 * Salida: Ninguna
 * Descripción: Elimina un producto de la lista de cotización por su código
 */
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

/**
 * reducirAumentarCantidades
 * Entrada: cantidad (entero), idProducto (string), lista (puntero a producto)
 * Salida: Ninguna
 * Descripción: Ajusta la cantidad de un producto en la cotización (suma/resta)
 */
void reducirAumentarCantidades(int cantidad, char* idProducto, producto* lista) {
    producto* actual = lista;
    int productoEncontrado = 0; 
    while (actual != NULL) {
        if (strcmp(actual->codigoProducto, idProducto) == 0) {
            productoEncontrado = 1;
            actual->cantidadProducto += cantidad;
            if (actual->cantidadProducto < 0) {
                printf("Advertencia: La cantidad no puede ser negativa. Se ajustará a 0.\n");
                actual->cantidadProducto = 0;
            }
            
            printf("Cantidad actualizada para producto %s: %d\n", 
                   idProducto, actual->cantidadProducto);
            break;
        }
        actual = actual->siguiente;
    }
    
    if (!productoEncontrado) {
        printf("Error: Producto con código %s no encontrado en la lista.\n", idProducto);
    }
}

/**
 * agregarCodigoEliminar
 * Entrada: lista (doble puntero a codigoProducto), codigo (string)
 * Salida: Ninguna
 * Descripción: Agrega un código de producto a la lista de códigos para eliminar
 */
void agregarCodigoEliminar(codigoProducto** lista, const char* codigo) {
    if (codigo == NULL || strlen(codigo) == 0) {
        return;
    }
    codigoProducto* nuevo = (codigoProducto*)malloc(sizeof(codigoProducto));
    if (nuevo == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el código\n");
        return;
    }
    nuevo->codigo = strdup(codigo);
    if (nuevo->codigo == NULL) {
        free(nuevo);
        fprintf(stderr, "Error: No se pudo asignar memoria para el código\n");
        return;
    }
    nuevo->siguiente = *lista;
    *lista = nuevo;
}

/**
 * verificarStock
 * Entrada: listaElementos (puntero a producto)
 * Salida: Ninguna
 * Descripción: Verifica y ajusta las cantidades de productos según el stock disponible
 */
void verificarStock(producto* listaElementos) {
    producto* actual = listaElementos;

    while (actual != NULL) {
        
        int cantidadDisponible = obtenerCantidadDisponible(actual->codigoProducto);
        printf("%i\n", cantidadDisponible);
        if (actual->cantidadProducto > cantidadDisponible) {
            printf("Producto: %s\n", actual->nombreProducto);
            printf("Cantidad solicitada: %d\n", actual->cantidadProducto);
            printf("Cantidad disponible en stock: %d\n", cantidadDisponible);
            printf("¿Desea hacer el ajuste? (s/n): ");
            char respuesta;
            scanf(" %c", &respuesta);
            if (respuesta == 's' || respuesta == 'S') {
                printf("Se ha ajustado la cantidad solicitada a: %d\n", cantidadDisponible);
                actual->cantidadProducto = cantidadDisponible;
            } else {
                actual->cantidadProducto = 0;
                printf("No se realiza el ajuste.\n");
            }
        }
        rebajarStock(actual->codigoProducto,actual->cantidadProducto);
        actual = actual->siguiente;
    }
}

/**
 * calcularTotal
 * Entrada: listaElementos (puntero a producto)
 * Salida: double (total calculado)
 * Descripción: Calcula el total de la cotización sumando precios*cantidades
 */
double calcularTotal(producto *listaElementos) {
    double total = 0.0;
    producto *actual = listaElementos;

    while (actual != NULL) {
        total += actual->precio * actual->cantidadProducto;  
        actual = actual->siguiente;  
    }

    return total;  
}