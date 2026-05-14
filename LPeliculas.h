//
// Created by smora on 6/05/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_CLECTURAANDPROCESAMIENTO_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_CLECTURAANDPROCESAMIENTO_H

#include "CClases.h"
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

//Flujo para lectura y registro inicial de peliculas

bool leerFilaCSV(ifstream& file, string& lineaCompleta);
vector<string> parseCSVLine(const string& linea);
unordered_map<int,Movie> leerPeliculas(const string& csv);



#endif //PROYECTO_1_PROGRAMACION_III_UTEC_CLECTURAANDPROCESAMIENTO_H
