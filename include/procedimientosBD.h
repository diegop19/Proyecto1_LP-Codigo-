#ifndef PROCEDIMIENTOSBD_H
#define PROCEDIMIENTOSBD_H
#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include "estructuras.h"

void insertarProductoBD(char *id, char *nombre, char *familia, double costo, double precio, int cantidad_stock);
void insertarFamiliaBD(char* idFamilia, char* descripcion);
int conectar(MYSQL **conexion);
void actualizarStockBD(char* id, int cantidad);
void desplegarProductos();
void filtrarBusqueda(char* identificador);
producto* obtenerProductoIndividual(char* codigo);
void guardarDatosCotizacion(producto* lista);
void agregarProductosCot(int numCotizacion, producto* lista);
void mostrarEstadisticasCotizaciones();
void mostrarTopProductos();
void mostrarVentasPorFamilia();
producto* datosCotizacion(int numCotizaion);
producto* crearProducto(const char* codigo, const char* nombre, int cantidad, double precio) ;
int actualizarCotizacionBD(int idCotizacion, producto* lista);
void eliminarProductosLista(int idCotizacion, codigoProducto* lista);
int eliminarProductoBD(char* identificador);
int obtenerNumeroFactura();
int obtenerCantidadDisponible(const char *idProducto);
void rebajarStock(const char *codigoProducto, int cantidadRebajar);
void actualizarFactura(int identificadorFactura, double subtotal, double total);
void facturarProductos(int numCotizacion,producto* lista);
void desplegarFacturas();
void desplegarDetallesFactura(int factura_id);
int actualizarEstadoCotizacionBD(int numCotizacion);
void mostrarResultado(const char* consulta, const char* mensaje, int esDecimal);
int agregarClienteBD(int numCedula, const char *nombre, const char *apellido);
int agregarFacturaACliente(int idCliente, int idFactura);
int eliminarFactura(int idFactura);
int validarCredenciales(char* nombreUsuario, char* clave);
#endif