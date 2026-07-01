#include "Memento.h"

static string fechaActual() {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm tmBuf;
    localtime_s(&tmBuf, &time);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tmBuf);
    return buffer;
}

HistorialEntry::HistorialEntry() : movieId(-1), tipo(""), detalle(""), timestamp("") {}

HistorialEntry::HistorialEntry(int movieId, const string& tipo, const string& detalle)
    : movieId(movieId), tipo(tipo), detalle(detalle), timestamp(fechaActual()) {}

Memento::Memento() = default;

Memento::Memento(const vector<HistorialEntry>& estado, const string& fecha)
    : estado(estado), fecha(fecha) {}

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
    if (indice < historial.size()) {
        return historial[indice];
    }
    return Memento();
}

size_t HistorialCareTaker::cantidad() const {
    return historial.size();
}

vector<Memento> HistorialCareTaker::getMementos() const {
    return historial;
}
