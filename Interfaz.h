#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "IUsuarios.h"
#include "LimPelis.h"
#include "Utilidades.h"
#include "Procesador.h"
#include "Iterator.h"
#include "Memento.h"
#include "Observer.h"
#include "Proxy.h"
#include "Command.h"

void limpiarPantalla();
void moverCursor(int x, int y);
void esperarEnter();
void setColor(int color);
void Home();

void mostrarMenu(int opcion);
void seleccionar_opcion(char& op);

void mostrar_inicio_sesion();
void inicio_sesion(string& correo, string& pass);
void esperar(int n);
void mostrar_registro_usuario();
void registro(string& correo, string& pass, string& name, string& clave, string& fechaNac);
void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, string& us_fechaNac, char& opcion_entrada);
void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, char& n, int edadUsuario = 18, size_t cantidadNotificaciones = 0);
void mostrarPerfilUsuario(string& email, string& password, string& nombre, string& fechaNac, int& edad);

void interfaz_buscar(unordered_map<int, Movie>& pelis, Procesador& pre_procesador, const string& emailUsuario, const string& nombreUsuario, int edadUsuario = 18);



#endif //PROYECTO_1_PROGRAMACION_III_UTEC_INTERFAZ_H
