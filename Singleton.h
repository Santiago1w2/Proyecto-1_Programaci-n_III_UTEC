#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H

#include "LimPelis.h"
#include "Procesador.h"

class CatalogoPeliculas {
    unordered_map<int, Movie> dataSucia;
    unordered_map<int, DataLimpia> dataLimpia;
    Procesador procesador;
    bool inicializado = false;

    CatalogoPeliculas() = default;
public:
    static CatalogoPeliculas& instancia();
    CatalogoPeliculas(const CatalogoPeliculas&) = delete;
    CatalogoPeliculas& operator=(const CatalogoPeliculas&) = delete;

    void cargarDatos(const string& archivoPeliculas, const string& archivoSalida);
    vector<int> buscar(const string& consulta);
    const unordered_map<int, Movie>& peliculas() const;
    unordered_map<int, Movie>& peliculas();
    Procesador& motorBusqueda();
    bool estaInicializado() const;
};

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H
