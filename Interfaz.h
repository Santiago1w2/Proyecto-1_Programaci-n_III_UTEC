//
// Created by smora on 29/04/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H

#include "CClases.h"
#include <iostream>
void limpiarPantalla();
void moverCursor(int x, int y);
void esperarEnter();
void setColor(int color);
void inicio();

void mostrarMenu(int opcion);
void seleccionar_opcion(char& op);

void mostrar_inicio_sesion();
void inicio_sesion(string& correo, string& pass);
void esperar(int n);
void mostrar_registro_usuario();
void registro(string& correo, string& pass, string& name, string& clave);



#endif //PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
