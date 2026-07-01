#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H

#include "Benchmark.h"
#include "LimPelis.h"
#include "Procesador.h"

class Singleton {
    unordered_map<int, Movie> dataSucia;
    unordered_map<int, DataLimpia> dataLimpia;
    Procesador procesador;
    bool inicializado = false;

    Singleton();
public:
    static Singleton& getInstancia();
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    void cargarDatos(const string& archivoPeliculas, const string& archivoSalida = "");
    vector<int> buscar(const string& consulta);
    const DataLimpia* obtenerPelicula(int id) const;
    const Movie* obtenerPeliculaOriginal(int id) const;
    const unordered_map<int, DataLimpia>& obtenerDataLimpia() const;
    const unordered_map<int, Movie>& obtenerDataSucia() const;
    vector<ResultadoBenchmark> ejecutarBenchmark();
    bool estaInicializado() const;
};

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H
