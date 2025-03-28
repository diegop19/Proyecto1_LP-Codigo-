#ifndef PROCEDIMIENTOSBD_H
#define PROCEDIMIENTOSBD_H
#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>

void insertarProductoBD(char *id, char *nombre, char *familia, double costo, double precio, int cantidad_stock);
void insertarFamiliaBD(char* idFamilia, char* descripcion);
int conectar(MYSQL **conexion);
void actualizarStockBD(char* id, int cantidad);
#endif