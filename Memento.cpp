#include "Memento.h"

HistorialEntry::HistorialEntry() : movieId(-1), tipo(""), query("") {}

HistorialEntry::HistorialEntry(int _movieId, const string& _tipo, const string& _query)
    : movieId(_movieId), tipo(_tipo), query(_query) {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm tmBuf;
    localtime_s(&tmBuf, &time);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tmBuf);
    timestamp = buffer;
}

Memento::Memento(const vector<HistorialEntry>& _estado, const string& _fecha)
    : estado(_estado), fecha(_fecha) {}

vector<HistorialEntry> Memento::getEstado() const {
    return estado;
}

string Memento::getFecha() const {
    return fecha;
}

void HistorialCareTaker::guardar(const vector<HistorialEntry>& estado) {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm tmBuf;
    localtime_s(&tmBuf, &time);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tmBuf);
    historial.emplace_back(estado, buffer);
}

Memento HistorialCareTaker::restaurar(int indice) const {
    if (indice >= 0 && indice < (int)historial.size())
        return historial[indice];
    return Memento({}, "");
}

int HistorialCareTaker::cantidad() const {
    return historial.size();
}

vector<Memento> HistorialCareTaker::getMementos() const {
    return historial;
}
