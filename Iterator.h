#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_ITERATOR_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_ITERATOR_H

#include "Memento.h"

class ResultadosIterator {
    vector<int> resultados;
    size_t posicion = 0;
    size_t tamanioPagina = 5;
public:
    explicit ResultadosIterator(vector<int> resultados, size_t tamanioPagina = 5);
    bool estaVacio() const;
    bool tieneSiguientePagina() const;
    bool tienePaginaAnterior() const;
    vector<int> paginaActual() const;
    vector<int> siguientePagina();
    vector<int> paginaAnterior();
    void reiniciar();
    size_t paginaActualNumero() const;
    size_t totalPaginas() const;
    size_t totalResultados() const;
};

class HistorialIterator {
    vector<HistorialEntry> entries;
    size_t posicion = 0;
public:
    explicit HistorialIterator(const vector<HistorialEntry>& entries);
    HistorialEntry siguiente();
    bool haySiguiente() const;
    bool estaVacio() const;
    void reiniciar();
    HistorialIterator filtrarPorTipo(const string& tipo) const;
    size_t total() const;
};

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_ITERATOR_H
