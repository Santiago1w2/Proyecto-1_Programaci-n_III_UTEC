#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>
#include "CClases.h"

using namespace std;

// Declaraciones de funciones de procesamiento y lectura CSV
bool leerFilaCSV(ifstream& file, string& lineaCompleta);
vector<string> parseCSVLine(const string& linea);

string aMinusculas(string texto);
string limpiarParentesisBasura(const string& s);

// Toma el mapa de películas, limpia el plot y retorna un mapa con los textos listos para el motor de búsqueda
unordered_map<int, string> prepararDataLimpia(const unordered_map<int, Movie>& pelis);

unordered_map<int, DataLimpia> construirDataLimpia(const unordered_map<int, Movie> &pelis);

#endif //PROCESAMIENTO_H