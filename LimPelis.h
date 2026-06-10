#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include "Utilidades.h"

//Flujo para lectura y registro inicial de peliculas
bool leerFilaCSV(ifstream& file, string& lineaCompleta);
vector<string> parseCSVLine(const string& linea);
unordered_map<int,Movie> leerPeliculas(const string& csv);




//Flujo para limpieza de peliculas

string aMinusculas(string texto);

// Función maestra reutilizable para columnas complejas.
string limpiarTextoAvanzado(const string& s, const vector<string>& parentesisProhibidos, const unordered_set<string>& palabrasProhibidas);

// Funciones exclusivas.
string limpiarTitulo(const string& s);
string limpiarOrigen(const string& s);
string limpiarDirector(const string& s);
string limpiarCast(const string& s);



// Función de limpieza genérica, se usará temporalmente para las demás columnas.
string normalizarYLimpiar(const string& s);

// Función para exportar el CSV separado en columnas limpias.
void exportarDataLimpiaCSV(unordered_map<int, Movie>& pelis, const string& nombreArchivo, unordered_map<int, DataLimpia>& datalimpia);


#endif //PROCESAMIENTO_H
