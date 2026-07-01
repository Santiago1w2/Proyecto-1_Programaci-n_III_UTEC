#include "Memento.h"

static string fechaActual() {
    auto ahora = chrono::system_clock::now();
    auto tiempo = chrono::system_clock::to_time_t(ahora);
    tm tmBuf{};
    localtime_s(&tmBuf, &tiempo);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tmBuf);
    return buffer;
}

HistorialEntry::HistorialEntry() : movieId(-1), tipo(""), detalle(""), timestamp("") {}

HistorialEntry::HistorialEntry(int _movieId, const string& _tipo, const string& _detalle)
    : movieId(_movieId), tipo(_tipo), detalle(_detalle), timestamp(fechaActual()) {}

Memento::Memento() = default;

Memento::Memento(const vector<HistorialEntry>& _estado, const string& _fecha)
    : estado(_estado), fecha(_fecha) {}

vector<HistorialEntry> Memento::getEstado() const {
    return estado;
}

string Memento::getFecha() const {
    return fecha;
}

void HistorialCareTaker::guardar(const vector<HistorialEntry>& estado) {
    historial.emplace_back(estado, fechaActual());
}

Memento HistorialCareTaker::restaurar(size_t indice) const {
    if (indice >= historial.size()) {
        return {};
    }
    return historial[indice];
}

size_t HistorialCareTaker::cantidad() const {
    return historial.size();
}

vector<Memento> HistorialCareTaker::getMementos() const {
    return historial;
}
