#include "Singleton.h"

Singleton::Singleton() = default;

Singleton& Singleton::getInstancia() {
    static Singleton instancia;
    return instancia;
}

void Singleton::cargarDatos(const string& archivoPeliculas, const string& archivoSalida) {
    if (inicializado) {
        return;
    }

    cout << "Leyendo archivo: " << archivoPeliculas << endl;
    dataSucia = leerPeliculas(archivoPeliculas);
    if (dataSucia.empty()) {
        throw runtime_error("No se cargaron peliculas desde el dataset.");
    }

    cout << "Limpiando datos..." << endl;
    exportarDataLimpiaCSV(dataSucia, archivoSalida, dataLimpia);
    if (dataLimpia.empty()) {
        throw runtime_error("La limpieza no produjo datos indexables.");
    }

    cout << "Construyendo indice paralelo..." << endl;
    procesador.procesar(dataLimpia);
    inicializado = true;
}

vector<int> Singleton::buscar(const string& consulta) {
    if (!inicializado) {
        throw logic_error("El motor de busqueda no esta inicializado.");
    }
    return procesador.buscar(consulta);
}

const DataLimpia* Singleton::obtenerPelicula(int id) const {
    auto it = dataLimpia.find(id);
    if (it == dataLimpia.end()) {
        return nullptr;
    }
    return &it->second;
}

const Movie* Singleton::obtenerPeliculaOriginal(int id) const {
    auto it = dataSucia.find(id);
    if (it == dataSucia.end()) {
        return nullptr;
    }
    return &it->second;
}

const unordered_map<int, DataLimpia>& Singleton::obtenerDataLimpia() const {
    return dataLimpia;
}

const unordered_map<int, Movie>& Singleton::obtenerDataSucia() const {
    return dataSucia;
}

vector<ResultadoBenchmark> Singleton::ejecutarBenchmark() {
    if (dataLimpia.empty()) {
        throw logic_error("No hay datos limpios para ejecutar benchmark.");
    }

    vector<ResultadoBenchmark> resultados;
    resultados.push_back(medirTiempo("Indexacion secuencial", dataLimpia.size(), [&]() {
        Procesador secuencial(1);
        secuencial.procesarSecuencial(dataLimpia);
    }));

    resultados.push_back(medirTiempo("Indexacion paralela", dataLimpia.size(), [&]() {
        Procesador paralelo;
        paralelo.procesar(dataLimpia);
    }));

    return resultados;
}

bool Singleton::estaInicializado() const {
    return inicializado;
}
