#ifndef LISTAS_H
#define LISTAS_H
#include "estructuras.h"
#include "procedimientosBD.h"
#include <stdlib.h>
#include <stdio.h>


void insertarProductoCotizacion(producto* nuevoProducto, producto** lista);
void verificarExistencia(char* codigo, int cantidad,producto** lista);
void removerProductoCot(char* codigo, producto** lista);
void reducirAumentarCantidades(int cantidad,char* idProducto, producto* lista);
void agregarCodigoEliminar(codigoProducto** lista, const char* codigo);
void verificarStock(producto* listaElementos);
double calcularTotal(producto *listaElementos);
#endif