#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H


#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "IUsuarios.h"
#include "LimPelis.h"
#include "Utilidades.h"


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
void registro(string& correo, string& pass, string& name, string& clave, string& edad);
void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, string& us_edad, char& opcion_entrada, const unordered_map<int, Movie>& pelis);
void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, int edad, char& n);



#endif //PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
