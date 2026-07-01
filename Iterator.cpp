#include "Iterator.h"

HistorialIterator::HistorialIterator(const vector<HistorialEntry>& _entries)
    : entries(_entries) {}

HistorialEntry HistorialIterator::primero() {
    posicion = 0;
    if (entries.empty()) {
        return {};
    }
    return entries[0];
}

HistorialEntry HistorialIterator::siguiente() {
    if (!haySiguiente()) {
        return {};
    }
    return entries[posicion++];
}

bool HistorialIterator::haySiguiente() const {
    return posicion < entries.size();
}

bool HistorialIterator::estaVacio() const {
    return entries.empty();
}

void HistorialIterator::reiniciar() {
    posicion = 0;
}

HistorialIterator HistorialIterator::filtrarPorTipo(const string& tipo) const {
    vector<HistorialEntry> filtrado;
    for (const auto& entry : entries) {
        if (entry.tipo == tipo) {
            filtrado.push_back(entry);
        }
    }
    return HistorialIterator(filtrado);
}

size_t HistorialIterator::total() const {
    return entries.size();
}
