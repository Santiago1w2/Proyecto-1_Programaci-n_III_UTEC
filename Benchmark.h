#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_BENCHMARK_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_BENCHMARK_H

#include "Utilidades.h"

struct ResultadoBenchmark {
    string nombre;
    long long milisegundos;
    size_t elementos;
};

template <typename Funcion>
ResultadoBenchmark medirTiempo(const string& nombre, size_t elementos, Funcion&& funcion) {
    auto inicio = chrono::high_resolution_clock::now();
    funcion();
    auto fin = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(fin - inicio).count();
    return {nombre, ms, elementos};
}

template <typename T, typename Comparador>
vector<T> tomarTopN(vector<T> valores, size_t n, Comparador comparador) {
    sort(valores.begin(), valores.end(), comparador);
    if (valores.size() > n) {
        valores.resize(n);
    }
    return valores;
}

#endif
