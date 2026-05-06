#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <cctype>
#include "CClases.h"

using namespace std;
// Declaraciones de funciones de procesamiento y lectura CSV
bool leerFilaCSV(ifstream& file, string& lineaCompleta);
vector<string> parseCSVLine(const string& linea);

string aMinusculas(string texto);

// NUEVA: Función maestra reutilizable para columnas complejas
string limpiarTextoAvanzado(const string& s, const vector<string>& parentesisProhibidos, const unordered_set<string>& palabrasProhibidas);

// Funciones exclusivas (Ahora usan la función maestra o su lógica rápida)
string limpiarTitulo(const string& s);
string limpiarOrigen(const string& s);
string limpiarDirector(const string& s); // NUEVA FUNCIÓN PARA DIRECTOR
string limpiarCast(const string& s);

// Función de limpieza genérica (se usará temporalmente para las demás columnas)
string normalizarYLimpiar(const string& s);

// Toma el mapa de películas, limpia las columnas y retorna un mapa con los textos para indexar
unordered_map<int, string> prepararDataLimpia(const unordered_map<int, Movie>& pelis);

// Función para exportar el CSV separado en columnas limpias
void exportarDataLimpiaCSV(const unordered_map<int, Movie>& pelis, const string& nombreArchivo, std::unordered_map<int, DataLimpia>& datalimpia);


#endif //PROCESAMIENTO_H