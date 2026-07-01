#include "Singleton.h"

static bool existeArchivo(const string& ruta) {
    ifstream archivo(ruta);
    return archivo.good();
}

static string resolverRutaPeliculas(const string& rutaSolicitada) {
    const vector<string> candidatos = {
        rutaSolicitada,
        "cmake-build-debug/" + rutaSolicitada,
        "../" + rutaSolicitada,
        "../cmake-build-debug/" + rutaSolicitada
    };

    for (const string& candidato : candidatos) {
        if (existeArchivo(candidato)) {
            return candidato;
        }
    }
    return rutaSolicitada;
}

CatalogoPeliculas& CatalogoPeliculas::instancia() {
    static CatalogoPeliculas catalogo;
    return catalogo;
}

void CatalogoPeliculas::cargarDatos(const string& archivoPeliculas, const string& archivoSalida) {
    const string rutaPeliculas = resolverRutaPeliculas(archivoPeliculas);
    dataSucia = leerPeliculas(rutaPeliculas);
    if (dataSucia.empty()) {
        throw runtime_error("No se cargaron peliculas desde el CSV.");
    }

    exportarDataLimpiaCSV(dataSucia, archivoSalida, dataLimpia);
    if (dataLimpia.empty()) {
        throw runtime_error("No se pudo construir la data limpia para busqueda.");
    }

    procesador.procesar(dataLimpia);
    inicializado = true;
}

vector<int> CatalogoPeliculas::buscar(const string& consulta) {
    if (!inicializado) {
        throw logic_error("El catalogo no esta inicializado.");
    }
    return procesador.buscar(consulta);
}

const unordered_map<int, Movie>& CatalogoPeliculas::peliculas() const {
    return dataSucia;
}

unordered_map<int, Movie>& CatalogoPeliculas::peliculas() {
    return dataSucia;
}

Procesador& CatalogoPeliculas::motorBusqueda() {
    return procesador;
}

bool CatalogoPeliculas::estaInicializado() const {
    return inicializado;
}
