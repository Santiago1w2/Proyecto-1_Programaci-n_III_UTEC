#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_IUSUARIOS_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_IUSUARIOS_H

#include "Utilidades.h"
#include "Interfaz.h"
#include "LimPelis.h"


string aMinusculas(string texto);
unordered_map<int, Movie> convertirAPelis(const vector<int>& ids, const unordered_map<int, Movie>& pelis);
vector<Usuario> leerUsuarios(const string &csv, const unordered_map<int,Movie>& pelis);

vector<int> parseLista(const string& s);

bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);

void registrar_nuevoUsuario(const string& name, const string& email, const string& clave);

void peliculasRecomendadasPanel(const unordered_map<int, Movie>& pelis);
string UserName(const string& _email, const string& _clave);
vector<string> mostrar_usuarios();


#endif //PROYECTO_1_PROGRAMACION_III_UTEC_IUSUARIOS_H
