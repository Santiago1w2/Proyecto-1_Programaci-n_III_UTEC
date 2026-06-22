#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_MEMENTO_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_MEMENTO_H

#include "Utilidades.h"

struct HistorialEntry {
    int movieId;
    string tipo;
    string query;
    string timestamp;

    HistorialEntry();
    HistorialEntry(int _movieId, const string& _tipo, const string& _query);
};

class Memento {
    vector<HistorialEntry> estado;
    string fecha;
public:
    Memento(const vector<HistorialEntry>& _estado, const string& _fecha);
    vector<HistorialEntry> getEstado() const;
    string getFecha() const;
};

class HistorialCareTaker {
    vector<Memento> historial;
public:
    void guardar(const vector<HistorialEntry>& estado);
    Memento restaurar(int indice) const;
    int cantidad() const;
    vector<Memento> getMementos() const;
};

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_MEMENTO_H
