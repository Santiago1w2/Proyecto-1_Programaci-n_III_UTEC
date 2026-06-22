#include "Iterator.h"

HistorialIterator::HistorialIterator(const vector<HistorialEntry>& _entries)
    : entries(_entries), posicion(0) {}

HistorialEntry HistorialIterator::primero() {
    posicion = 0;
    if (entries.empty()) return HistorialEntry();
    return entries[0];
}

HistorialEntry HistorialIterator::siguiente() {
    if (posicion < (int)entries.size())
        return entries[posicion++];
    return HistorialEntry();
}

bool HistorialIterator::haySiguiente() const {
    return posicion < (int)entries.size();
}

bool HistorialIterator::estaVacio() const {
    return entries.empty();
}

void HistorialIterator::reiniciar() {
    posicion = 0;
}

HistorialIterator HistorialIterator::filtrarPorTipo(const string& tipo) const {
    vector<HistorialEntry> filtrado;
    for (const auto& e : entries) {
        if (e.tipo == tipo)
            filtrado.push_back(e);
    }
    return HistorialIterator(filtrado);
}

int HistorialIterator::total() const {
    return entries.size();
}
