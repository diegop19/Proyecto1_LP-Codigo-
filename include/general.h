#ifndef GENERAL_H
#define GENERAL_H
#include "listas.h"
char* obtenerInput();
void menuGeneral();
void consultarCatalogo();
void cotizar();
void modificarCotizacion();
void facturar();
void mostrarEstadisticas();
void agregarProductoCotizacion();
void verProductosEnCotizacion(producto **lista);
void removerProductoCotizacion();
void guardarDatosCotizacion();
void guardarCambios(int idCotizacion, producto* lista);
#endif