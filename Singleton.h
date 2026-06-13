//
// Created by smora on 9/06/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H

#include "LimPelis.h"
#include "Procesador.h"

// Patron Singleton aplicado al motor de busqueda de peliculas.
// La idea es que todo el programa use una sola instancia de este gestor,
// porque el Trie y la data limpia pesan bastante y no conviene cargarlos
// o procesarlos varias veces en distintas partes del sistema.
class Singleton {
private:
    unordered_map<int, DataLimpia> dataLimpia;
    Procesador procesador;
    bool inicializado = false;

    // Constructor privado: nadie puede crear objetos Singleton con "new"
    // ni como variable local. La unica entrada valida es getInstancia().
    Singleton();

public:
    // Devuelve siempre el mismo objeto. En C++, una variable static local
    // se crea una sola vez y vive hasta que termina el programa.
    static Singleton& getInstancia();

    // Se eliminan copia y asignacion para garantizar que no existan clones
    // del gestor. Asi se mantiene una unica fuente de datos y un unico Trie.
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Conecta el Singleton con el flujo del proyecto:
    // 1. lee peliculas.csv,
    // 2. genera datosLimpios.csv y llena dataLimpia en memoria,
    // 3. procesa esa data dentro del Procesador, que internamente arma los Tries.
    void cargarDatos(const string& archivoPeliculas, const string& archivoSalida);

    // Punto de acceso para buscar peliculas desde cualquier modulo sin crear
    // otro Procesador. Internamente delega la consulta al Trie del procesador.
    vector<int> buscar(const string& consulta);

    // Permite obtener la informacion limpia de una pelicula por id para mostrar
    // titulos, detalles, etc. Devuelve nullptr si el id no existe.
    const DataLimpia* obtenerPelicula(int id) const;

    bool estaInicializado() const;
};



#endif //PROYECTO_1_PROGRAMACION_III_UTEC_SINGLETON_H
