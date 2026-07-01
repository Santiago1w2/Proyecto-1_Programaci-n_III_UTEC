#include "Iterator.h"

ResultadosIterator::ResultadosIterator(vector<int> resultados, size_t tamanioPagina)
    : resultados(move(resultados)), tamanioPagina(tamanioPagina == 0 ? 5 : tamanioPagina) {}

bool ResultadosIterator::estaVacio() const {
    return resultados.empty();
}

bool ResultadosIterator::tieneSiguientePagina() const {
    return posicion + tamanioPagina < resultados.size();
}

bool ResultadosIterator::tienePaginaAnterior() const {
    return posicion >= tamanioPagina;
}

vector<int> ResultadosIterator::paginaActual() const {
    vector<int> pagina;
    if (resultados.empty()) {
        return pagina;
    }

    const size_t fin = min(posicion + tamanioPagina, resultados.size());
    for (size_t i = posicion; i < fin; ++i) {
        pagina.push_back(resultados[i]);
    }
    return pagina;
}

vector<int> ResultadosIterator::siguientePagina() {
    if (tieneSiguientePagina()) {
        posicion += tamanioPagina;
    }
    return paginaActual();
}

vector<int> ResultadosIterator::paginaAnterior() {
    if (tienePaginaAnterior()) {
        posicion -= tamanioPagina;
    }
    return paginaActual();
}

void ResultadosIterator::reiniciar() {
    posicion = 0;
}

size_t ResultadosIterator::paginaActualNumero() const {
    if (resultados.empty()) {
        return 0;
    }
    return (posicion / tamanioPagina) + 1;
}

size_t ResultadosIterator::totalPaginas() const {
    if (resultados.empty()) {
        return 0;
    }
    return (resultados.size() + tamanioPagina - 1) / tamanioPagina;
}

size_t ResultadosIterator::totalResultados() const {
    return resultados.size();
}

HistorialIterator::HistorialIterator(const vector<HistorialEntry>& entries)
    : entries(entries) {}

HistorialEntry HistorialIterator::siguiente() {
    if (posicion < entries.size()) {
        return entries[posicion++];
    }
    return HistorialEntry();
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
