//
// Created by smora on 9/06/2026.
//

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

    cout << "Leyendo archivos..." << endl;
    unordered_map<int, Movie> dataSucia = leerPeliculas(archivoPeliculas);

    cout << "Limpiando datos y preparando texto..." << endl;
    exportarDataLimpiaCSV(dataSucia, archivoSalida, dataLimpia);

    cout << "Procesando datos para subir al Trie..." << endl;
    auto inicio = chrono::high_resolution_clock::now();
    procesador.procesar(dataLimpia);
    auto fin = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);

    cout << "Indice listo en " << duracion.count() << " ms" << endl;
    inicializado = true;
}

vector<int> Singleton::buscar(const string& consulta) {
    if (!inicializado) {
        return {};
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

bool Singleton::estaInicializado() const {
    return inicializado;
}
