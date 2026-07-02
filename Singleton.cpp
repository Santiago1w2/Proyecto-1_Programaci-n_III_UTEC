#include "Singleton.h"

static void registrarMetricaCatalogo(const string& operacion, long long tiempoMs, size_t cantidad = 0) {
    ofstream archivo("metricas_tiempo.txt", ios::app);
    if (!archivo.is_open()) {
        return;
    }

    archivo << operacion << " | tiempo_ms: " << tiempoMs;
    if (cantidad > 0) {
        archivo << " | cantidad: " << cantidad;
    }
    archivo << endl;
}

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
    const auto inicioTotal = chrono::high_resolution_clock::now();
    const string rutaPeliculas = resolverRutaPeliculas(archivoPeliculas);

    const auto inicioLectura = chrono::high_resolution_clock::now();
    dataSucia = leerPeliculas(rutaPeliculas);
    const auto finLectura = chrono::high_resolution_clock::now();
    registrarMetricaCatalogo(
        "LECTURA_CSV",
        chrono::duration_cast<chrono::milliseconds>(finLectura - inicioLectura).count(),
        dataSucia.size()
    );

    if (dataSucia.empty()) {
        throw runtime_error("No se cargaron peliculas desde el CSV.");
    }

    const auto inicioLimpieza = chrono::high_resolution_clock::now();
    exportarDataLimpiaCSV(dataSucia, archivoSalida, dataLimpia);
    const auto finLimpieza = chrono::high_resolution_clock::now();
    registrarMetricaCatalogo(
        "LIMPIEZA_EXPORTACION",
        chrono::duration_cast<chrono::milliseconds>(finLimpieza - inicioLimpieza).count(),
        dataLimpia.size()
    );

    if (dataLimpia.empty()) {
        throw runtime_error("No se pudo construir la data limpia para busqueda.");
    }

    procesador.procesar(dataLimpia);
    const auto finTotal = chrono::high_resolution_clock::now();
    registrarMetricaCatalogo(
        "CARGA_TOTAL_CATALOGO",
        chrono::duration_cast<chrono::milliseconds>(finTotal - inicioTotal).count(),
        dataLimpia.size()
    );
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
