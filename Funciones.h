//
// Created by smora on 6/05/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_CLECTURAANDPROCESAMIENTO_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_CLECTURAANDPROCESAMIENTO_H

#include "CClases.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Plimpieza.h"
#include "Interfaz.h"
using namespace std;
string aMinusculas(string texto);
unordered_map<int,Movie> leerPeliculas(const string& csv);
unordered_map<int, Movie> convertirAPelis(const vector<int>& ids, const unordered_map<int, Movie>& pelis);
vector<Usuario> leerUsuarios(const string &csv, const unordered_map<int,Movie>& pelis);

vector<int> parseLista(const string& s);

bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);
bool validar_usuario(const string& _username); //No es necesario

void registrar_nuevoUsuario(const string& name, const string& email, const string& clave);
void actualizarUsuario(vector<int> pelis,string tipo);

void peliculasRecomendadasPanel(const unordered_map<int, Movie>& pelis);
string UserName(const string& _email, const string& _clave);
vector<string> mostrar_usuarios();


#endif //PROYECTO_1_PROGRAMACION_III_UTEC_CLECTURAANDPROCESAMIENTO_H
