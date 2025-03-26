#ifndef PROCEDIMIENTOSBD_H
#define PROCEDIMIENTOSBD_H
#include <mysql.h>

void insertarProducto(char *id, char *nombre, char *familia, float costo, float precio, int cantidad_stock);
void insertarFamilia();
int conectar(MYSQL **conexion);
#endif