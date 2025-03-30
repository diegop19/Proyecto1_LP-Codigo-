#ifndef LISTAS_H
#define LISTAS_H
#include "estructuras.h"
#include "procedimientosBD.h"
#include <stdlib.h>
#include <stdio.h>


void insertarProductoCotizacion(producto* nuevoProducto, producto** lista);
void verificarExistencia(char* codigo, int cantidad,producto** lista);
void removerProductoCot(char* codigo, producto** lista);

#endif