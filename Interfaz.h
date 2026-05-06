//
// Created by smora on 29/04/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H

#include "Interfaz.h"
#include <conio.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "Funciones.h"
using namespace std;
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
void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, char& opcion_entrada);
void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, char& n);



#endif //PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
