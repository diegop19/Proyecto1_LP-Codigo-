#ifndef ADMIN_H
#define ADMIN_H

#define MAX_NOMBRE 50

struct FamiliaProducto {
    int id;                     
    char nombre[MAX_NOMBRE];    
};
void menuAdministrativo();
char* abrirExploradorArchivos();
void registrarFamiliaProductos();
void insertarFamilia(char *id, char *descripcion);

void registroDeProductosMenu();
void registrarProductos();
void insertarProducto(char *id, char *nombre, char *familia, float costo, float precio, int cantidad_stock);

void eliminarProducto();

void actualizarStock(char *id, int cantidad);
void cargarInventario();
void consultarFacturas();
void menuEstadisticas();
void mostrarEstadisticas();
void verificarCredenciales(char* usuario, char* contraseña);
#endif