#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_ITERATOR_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_ITERATOR_H

#include "Memento.h"

class HistorialIterator {
    vector<HistorialEntry> entries;
    size_t posicion = 0;
public:
    explicit HistorialIterator(const vector<HistorialEntry>& _entries);

    HistorialEntry primero();
    HistorialEntry siguiente();
    bool haySiguiente() const;
    bool estaVacio() const;
    void reiniciar();

    HistorialIterator filtrarPorTipo(const string& tipo) const;
    size_t total() const;
};

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_ITERATOR_H
