#include "Iterator.h"
#include "Interfaz.h"
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

static void mostrarPerfil(const string& nombre, const string& correo) {
    limpiarPantalla();
    cout << "===== MI PERFIL =====\n";
    cout << "Nombre: " << nombre << "\n";
    cout << "Correo: " << correo << "\n";
    esperarEnter();
}

static void mostrarHistorialSesion(const vector<HistorialEntry>& historial, const HistorialCareTaker& careTaker) {
    limpiarPantalla();
    cout << "===== HISTORIAL DE SESION =====\n";

    if (historial.empty()) {
        cout << "Aun no hay busquedas ni peliculas vistas en esta sesion.\n";
    } else {
        HistorialIterator it(historial);
        while (it.haySiguiente()) {
            HistorialEntry entry = it.siguiente();
            cout << entry.timestamp << " [" << entry.tipo << "] " << entry.detalle << "\n";
        }
        cout << "\nSnapshots guardados: " << careTaker.cantidad() << "\n";
    }

    esperarEnter();
}

static void mostrarRecomendaciones(const unordered_map<int, Movie>& pelis) {
    limpiarPantalla();
    cout << "===== RECOMENDACIONES =====\n\n";
    peliculasRecomendadasPanel(pelis);
    cout << "\n";
    esperarEnter();
}

static void buscarPelicula(Singleton& motor, int edadUsuario, vector<HistorialEntry>& historial, HistorialCareTaker& careTaker) {
    limpiarPantalla();
    cout << "===== BUSCAR PELICULA =====\n";
    cout << "Escribe 'volver' para regresar al menu.\n\n";

    string consulta;
    while (true) {
        cout << "Buscar: ";
        getline(cin, consulta);

        if (consulta == "volver" || consulta == "VOLVER") {
            break;
        }
        if (consulta.empty()) {
            cout << "Ingrese una consulta no vacia.\n";
            continue;
        }

        historial.emplace_back(-1, "busqueda", consulta);
        vector<int> resultados = motor.buscar(consulta);

        if (resultados.empty()) {
            cout << "Sin resultados.\n\n";
            careTaker.guardar(historial);
            continue;
        }

        cout << "\nResultados:\n";
        int mostrados = 0;
        for (int id : resultados) {
            const Movie* movie = motor.obtenerPeliculaOriginal(id);
            const DataLimpia* limpia = motor.obtenerPelicula(id);
            if (movie == nullptr) {
                continue;
            }

            PeliculaReal real(movie);
            PeliculaProxy proxy(&real, edadUsuario);
            const string titulo = limpia ? limpia->getTitle() : proxy.getTitulo();
            historial.emplace_back(id, "pelicula", titulo);

            cout << id << " -> " << proxy.getTitulo()
                 << " | " << proxy.getGenero() << "\n";

            if (++mostrados >= 15) {
                break;
            }
        }

        careTaker.guardar(historial);
        cout << "\n";
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

        if (soloBenchmark) {
            motor.cargarDatos(dataset, salidaLimpia);
            auto resultadosBenchmark = motor.ejecutarBenchmark();
            imprimirBenchmark(resultadosBenchmark);
            return 0;
        }

        inicio();

        string correo;
        string password;
        string nombre;
        char opcionEntrada = 'a';
        seleccionar_opcion(opcionEntrada);
        InicioSesionAndRegistro(correo, password, nombre, opcionEntrada);

        cout << "Cargando peliculas, espera un momento...\n";
        motor.cargarDatos(dataset, salidaLimpia);

        vector<HistorialEntry> historial;
        HistorialCareTaker careTaker;

        char opcionPrincipal = '\0';
        do {
            pantallaPrincipal(nombre, motor.obtenerDataSucia(), opcionPrincipal);
            opcionPrincipal = static_cast<char>(toupper(static_cast<unsigned char>(opcionPrincipal)));

            switch (opcionPrincipal) {
                case 'A':
                    mostrarPerfil(nombre, correo);
                    break;
                case 'B':
                    mostrarHistorialSesion(historial, careTaker);
                    break;
                case 'C':
                    mostrarRecomendaciones(motor.obtenerDataSucia());
                    break;
                case 'D':
                    buscarPelicula(motor, edadUsuario, historial, careTaker);
                    break;
                case '0':
                    break;
                default:
                    cout << "Opcion no valida.\n";
                    esperar(1);
                    break;
            }
        } while (opcionPrincipal != '0');

        limpiarPantalla();
        cout << "Gracias por usar UTECFLIX.\n";

        return 0;
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}
