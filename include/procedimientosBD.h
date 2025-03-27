#ifndef PROCEDIMIENTOSBD_H
#define PROCEDIMIENTOSBD_H
#include <mysql.h>

void insertarProducto(char *id, char *nombre, char *familia, float costo, float precio, int cantidad_stock);
void insertarFamilia(char* idFamilia, char* descripcion);
int conectar(MYSQL **conexion);
#endif