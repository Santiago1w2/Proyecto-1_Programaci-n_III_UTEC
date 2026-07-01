#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_MEMENTO_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_MEMENTO_H

#include "Utilidades.h"

struct HistorialEntry {
    int movieId;
    string tipo;
    string detalle;
    string timestamp;

    HistorialEntry();
    HistorialEntry(int _movieId, const string& _tipo, const string& _detalle);
};

class Memento {
    vector<HistorialEntry> estado;
    string fecha;
public:
    Memento();
    Memento(const vector<HistorialEntry>& _estado, const string& _fecha);
    vector<HistorialEntry> getEstado() const;
    string getFecha() const;
};

class HistorialCareTaker {
    vector<Memento> historial;
public:
    void guardar(const vector<HistorialEntry>& estado);
    Memento restaurar(size_t indice) const;
    size_t cantidad() const;
    vector<Memento> getMementos() const;
};

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_MEMENTO_H
