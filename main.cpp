#include "Iterator.h"
#include "Memento.h"
#include "Proxy.h"
#include "Singleton.h"

static string resolverDataset(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--age") {
            ++i;
            continue;
        }
        if (!arg.empty() && arg.rfind("--", 0) != 0) {
            return arg;
        }
    }

    const vector<string> candidatos = {
        "peliculas.csv",
        "wiki_movie_plots_deduped.csv",
        "wiki_movie_plots_deduped (2).csv"
    };

    for (const auto& candidato : candidatos) {
        if (filesystem::exists(candidato)) {
            return candidato;
        }
    }
    return candidatos.front();
}

static bool tieneOpcion(int argc, char* argv[], const string& opcion) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == opcion) {
            return true;
        }
    }
    return false;
}

static int leerEdad(int argc, char* argv[]) {
    for (int i = 1; i + 1 < argc; ++i) {
        if (string(argv[i]) == "--age") {
            try {
                return stoi(argv[i + 1]);
            } catch (...) {
                throw invalid_argument("La edad indicada en --age no es valida.");
            }
        }
    }
    return 18;
}

static void imprimirBenchmark(const vector<ResultadoBenchmark>& resultados) {
    cout << "\n===== BENCHMARK =====\n";
    cout << left << setw(28) << "Algoritmo"
         << right << setw(14) << "Peliculas"
         << setw(14) << "Tiempo(ms)" << endl;
    cout << string(56, '-') << endl;

    for (const auto& r : resultados) {
        cout << left << setw(28) << r.nombre
             << right << setw(14) << r.elementos
             << setw(14) << r.milisegundos << endl;
    }
}

int main(int argc, char* argv[]) {
    try {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        system("chcp 65001 > nul");
        system("cls");

        const string dataset = resolverDataset(argc, argv);
        const bool soloBenchmark = tieneOpcion(argc, argv, "--benchmark");
        const bool exportarLimpio = tieneOpcion(argc, argv, "--export-clean");
        const int edadUsuario = leerEdad(argc, argv);
        const string salidaLimpia = exportarLimpio ? "datosLimpios.csv" : "";

        Singleton& motor = Singleton::getInstancia();
        motor.cargarDatos(dataset, salidaLimpia);

        auto resultadosBenchmark = motor.ejecutarBenchmark();
        imprimirBenchmark(resultadosBenchmark);
        if (soloBenchmark) {
            return 0;
        }

        vector<HistorialEntry> historial;
        HistorialCareTaker careTaker;

        cout << "\n===== PRUEBA DEL TRIE =====\n";
        cout << "Dataset cargado: " << dataset << "\n";
        cout << "Peliculas indexadas: " << motor.obtenerDataLimpia().size() << "\n";
        cout << "Escribe 'exit' para salir.\n";

        string consulta;
        while(true)
        {
            cout << "\nBuscar: ";
            getline(cin, consulta);
            if(consulta == "exit")
                break;
            if (consulta.empty()) {
                cout << "Ingrese una consulta no vacia.\n";
                continue;
            }

            historial.emplace_back(-1, "busqueda", consulta);
            vector<int> resultados = motor.buscar(consulta);
            if(resultados.empty())
            {
                cout << "Sin resultados\n";
                continue;
            }

            cout << "\nResultados:\n";
            for(int id : resultados)
            {
                const Movie* movie = motor.obtenerPeliculaOriginal(id);
                const DataLimpia* limpia = motor.obtenerPelicula(id);
                PeliculaReal real(movie);
                PeliculaProxy proxy(&real, edadUsuario);
                historial.emplace_back(id, "pelicula", limpia ? limpia->getTitle() : proxy.getTitulo());

                cout << id << " -> " << proxy.getTitulo() << endl;
            }
            careTaker.guardar(historial);
        }

        if (!historial.empty()) {
            cout << "\n===== HISTORIAL DE SESION =====\n";
            HistorialIterator it(historial);
            while (it.haySiguiente()) {
                HistorialEntry entry = it.siguiente();
                cout << entry.timestamp << " [" << entry.tipo << "] " << entry.detalle << endl;
            }
            cout << "Snapshots guardados: " << careTaker.cantidad() << endl;
        }

        return 0;
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}
