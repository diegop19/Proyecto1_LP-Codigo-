#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
typedef struct producto{
    char* codigoProducto;
    char* nombreProducto;
    int cantidadProducto;
    double precio;
    struct producto* siguiente;
}producto;

#endif