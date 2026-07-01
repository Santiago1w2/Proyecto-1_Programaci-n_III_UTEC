#include "AutenticacionStrategy.h"
#include "IUsuarios.h"

const string ARCHIVO_PREGUNTAS = "preguntasRecuperacion.csv";

struct PreguntaRespuesta {
    string pregunta;
    string respuesta;
};

static string limpiarRespuesta(string texto) {
    texto = aMinuscula(texto);
    size_t inicio = texto.find_first_not_of(" \t\r\n");
    size_t fin = texto.find_last_not_of(" \t\r\n");
    if (inicio == string::npos) return "";
    return texto.substr(inicio, fin - inicio + 1);
}

static vector<string> separarCSVSimple(const string& linea) {
    vector<string> campos;
    string campo;
    stringstream ss(linea);
    while (getline(ss, campo, ',')) {
        campos.push_back(campo);
    }
    return campos;
}

static vector<string> parseListaBusquedas(const string& texto) {
    string contenido = texto;
    if (!contenido.empty() && contenido.front() == '[') contenido.erase(contenido.begin());
    if (!contenido.empty() && contenido.back() == ']') contenido.pop_back();

    vector<string> busquedas;
    string busqueda;
    stringstream ss(contenido);
    while (getline(ss, busqueda, ';')) {
        busqueda = limpiarRespuesta(busqueda);
        if (!busqueda.empty()) busquedas.push_back(busqueda);
    }
    return busquedas;
}

static bool obtenerDatosUsuario(const string& email, const string& clave, UsuarioAutenticado& usuario) {
    ifstream archivo("registroUsuarios.txt");
    string linea;

    while (getline(archivo, linea)) {
        vector<string> campos = separarCSVSimple(linea);
        if (campos.size() < 4) continue;
        if (campos[0] == email && campos[1] == clave) {
            usuario.autenticado = true;
            usuario.email = campos[0];
            usuario.password = campos[1];
            usuario.fechaNac = campos[2];
            usuario.nombre = campos[3];
            return true;
        }
    }
    return false;
}

static bool obtenerDatosUsuarioPorEmail(const string& email, UsuarioAutenticado& usuario) {
    ifstream archivo("registroUsuarios.txt");
    string linea;

    while (getline(archivo, linea)) {
        vector<string> campos = separarCSVSimple(linea);
        if (campos.size() < 4) continue;
        if (campos[0] == email) {
            usuario.autenticado = true;
            usuario.email = campos[0];
            usuario.password = campos[1];
            usuario.fechaNac = campos[2];
            usuario.nombre = campos[3];
            return true;
        }
    }
    return false;
}

static vector<PreguntaRespuesta> obtenerPreguntasUsuario(const string& email) {
    ifstream archivo(ARCHIVO_PREGUNTAS);
    vector<PreguntaRespuesta> preguntas;
    string linea;

    while (getline(archivo, linea)) {
        vector<string> campos = separarCSVSimple(linea);
        if (campos.size() < 3) continue;
        if (campos[0] == email) {
            preguntas.push_back({campos[1], campos[2]});
        }
    }
    return preguntas;
}

static bool tienePreguntasConfiguradas(const string& email) {
    return !obtenerPreguntasUsuario(email).empty();
}

static vector<string> obtenerBusquedasUsuario(const string& email) {
    ifstream archivo("registroUsuarios.txt");
    string linea;

    while (getline(archivo, linea)) {
        vector<string> campos = separarCSVSimple(linea);
        if (campos.size() < 8) continue;
        if (campos[0] == email) {
            return parseListaBusquedas(campos[7]);
        }
    }
    return {};
}

static bool tieneHistorialSuficiente(const string& email) {
    return obtenerBusquedasUsuario(email).size() >= 5;
}

static void pausar() {
    cout << "\nPresiona Enter para continuar...";
    cin.ignore();
    cin.get();
}

static void mostrarSinMetodoDisponible() {
    cout << "\nNo cuenta con otro metodo de autenticacion disponible.\n";
    pausar();
}

ContextoAutenticacion::ContextoAutenticacion(const unordered_map<int, Movie>& _peliculas)
    : peliculas(_peliculas) {
    estrategia = make_unique<AutenticacionContrasenaStrategy>();
}

UsuarioAutenticado ContextoAutenticacion::iniciar() {
    while (estrategia != nullptr) {
        UsuarioAutenticado usuario = estrategia->autenticar(*this);
        if (usuario.autenticado) return usuario;
        if (siguienteEstrategia != nullptr) {
            estrategia = move(siguienteEstrategia);
        } else {
            break;
        }
    }
    return {};
}

void ContextoAutenticacion::cambiarEstrategia(unique_ptr<AutenticacionStrategy> nuevaEstrategia) {
    siguienteEstrategia = move(nuevaEstrategia);
}

const unordered_map<int, Movie>& ContextoAutenticacion::getPeliculas() const {
    return peliculas;
}

string ContextoAutenticacion::getCorreoActual() const {
    return correoActual;
}

void ContextoAutenticacion::setCorreoActual(const string& correo) {
    correoActual = correo;
}

UsuarioAutenticado AutenticacionContrasenaStrategy::autenticar(ContextoAutenticacion& contexto) {
    string email, clave;

    system("cls");
    cout << "===== INICIO DE SESION: CONTRASENA =====\n";
    cout << "Correo: ";
    cin >> email;
    contexto.setCorreoActual(email);
    cout << "Contrasena: ";
    cin >> clave;

    UsuarioAutenticado usuario;
    if (obtenerDatosUsuario(email, clave, usuario)) {
        return usuario;
    }

    cout << "\nCredenciales incorrectas.\n";
    cout << "1. Intentar nuevamente\n";
    cout << "2. Olvide mi contrasena\n";
    cout << "Opcion: ";
    int opcion;
    cin >> opcion;

    if (opcion == 2) {
        if (tienePreguntasConfiguradas(email)) {
            contexto.cambiarEstrategia(make_unique<AutenticacionPreguntasStrategy>());
        } else if (tieneHistorialSuficiente(email)) {
            contexto.cambiarEstrategia(make_unique<AutenticacionHistorialStrategy>());
        } else {
            mostrarSinMetodoDisponible();
            contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
        }
    } else {
        contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
    }

    return {};
}

UsuarioAutenticado AutenticacionPreguntasStrategy::autenticar(ContextoAutenticacion& contexto) {
    string email = contexto.getCorreoActual();
    if (email.empty()) {
        cout << "Correo: ";
        cin >> email;
        contexto.setCorreoActual(email);
    }

    vector<PreguntaRespuesta> preguntas = obtenerPreguntasUsuario(email);
    if (preguntas.empty()) {
        cout << "\nEste usuario no activo preguntas de recuperacion.\n";
        if (tieneHistorialSuficiente(email)) {
            contexto.cambiarEstrategia(make_unique<AutenticacionHistorialStrategy>());
        } else {
            mostrarSinMetodoDisponible();
            contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
        }
        return {};
    }

    shuffle(preguntas.begin(), preguntas.end(), mt19937(random_device{}()));
    int cantidad = min(3, static_cast<int>(preguntas.size()));
    int correctas = 0;

    system("cls");
    cout << "===== INICIO DE SESION: PREGUNTAS =====\n";
    cin.ignore();
    for (int i = 0; i < cantidad; i++) {
        string respuesta;
        cout << preguntas[i].pregunta << ": ";
        getline(cin, respuesta);
        if (limpiarRespuesta(respuesta) == limpiarRespuesta(preguntas[i].respuesta)) {
            correctas++;
        }
    }

    if (correctas == cantidad && cantidad == 3) {
        UsuarioAutenticado usuario;
        obtenerDatosUsuarioPorEmail(email, usuario);
        return usuario;
    }

    cout << "\nRespuestas incorrectas.\n";
    cout << "1. Intentar preguntas nuevamente\n";
    cout << "2. Probar otro metodo\n";
    cout << "3. Regresar al metodo anterior\n";
    cout << "Opcion: ";
    int opcion;
    cin >> opcion;

    if (opcion == 2) {
        if (tieneHistorialSuficiente(email)) {
            contexto.cambiarEstrategia(make_unique<AutenticacionHistorialStrategy>());
        } else {
            cout << "\nEl metodo por historial no esta disponible.\n";
            mostrarSinMetodoDisponible();
            contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
        }
    } else if (opcion == 3) {
        contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
    } else {
        contexto.cambiarEstrategia(make_unique<AutenticacionPreguntasStrategy>());
    }

    return {};
}

UsuarioAutenticado AutenticacionHistorialStrategy::autenticar(ContextoAutenticacion& contexto) {
    string email = contexto.getCorreoActual();
    if (email.empty()) {
        cout << "Correo: ";
        cin >> email;
        contexto.setCorreoActual(email);
    }

    vector<string> historialBusquedas = obtenerBusquedasUsuario(email);
    if (historialBusquedas.size() < 5) {
        cout << "\nEl metodo por historial no esta disponible: necesita al menos 5 busquedas.\n";
        if (tienePreguntasConfiguradas(email)) {
            contexto.cambiarEstrategia(make_unique<AutenticacionPreguntasStrategy>());
        } else {
            mostrarSinMetodoDisponible();
            contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
        }
        return {};
    }

    vector<string> ultimasBusquedas;
    for (int i = static_cast<int>(historialBusquedas.size()) - 1; i >= 0 && ultimasBusquedas.size() < 5; i--) {
        ultimasBusquedas.push_back(limpiarRespuesta(historialBusquedas[i]));
    }

    system("cls");
    cout << "===== INICIO DE SESION: HISTORIAL =====\n";
    cout << "Ingresa 2 busquedas de tus ultimas 5 busquedas realizadas.\n";

    cin.ignore();
    string respuesta1, respuesta2;
    cout << "Busqueda 1: ";
    getline(cin, respuesta1);
    cout << "Busqueda 2: ";
    getline(cin, respuesta2);

    unordered_set<string> respuestas;
    respuestas.insert(limpiarRespuesta(respuesta1));
    respuestas.insert(limpiarRespuesta(respuesta2));

    int coincidencias = 0;
    for (const string& busqueda : ultimasBusquedas) {
        if (respuestas.count(busqueda)) coincidencias++;
    }

    if (coincidencias >= 2) {
        UsuarioAutenticado usuario;
        obtenerDatosUsuarioPorEmail(email, usuario);
        return usuario;
    }

    cout << "\nNo coinciden suficientes busquedas.\n";
    cout << "1. Intentar historial nuevamente\n";
    cout << "2. Regresar al metodo anterior\n";
    cout << "Opcion: ";
    int opcion;
    cin >> opcion;

    if (opcion == 2) {
        if (tienePreguntasConfiguradas(email)) {
            contexto.cambiarEstrategia(make_unique<AutenticacionPreguntasStrategy>());
        } else {
            contexto.cambiarEstrategia(make_unique<AutenticacionContrasenaStrategy>());
        }
    } else {
        contexto.cambiarEstrategia(make_unique<AutenticacionHistorialStrategy>());
    }

    return {};
}

void registrarPreguntasRecuperacion(const string& email) {
    vector<string> preguntas = {
        "Numero de celular",
        "Primer nombre de su padre",
        "Primer nombre de su madre",
        "Gaseosa favorita",
        "Videojuego favorito",
        "Deporte favorito",
        "Comida favorita"
    };

    cout << "\nDesea activar preguntas de recuperacion? (s/n): ";
    char opcion;
    cin >> opcion;
    if (opcion != 's' && opcion != 'S') return;

    ofstream archivo(ARCHIVO_PREGUNTAS, ios::app);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ARCHIVO_PREGUNTAS << endl;
        return;
    }

    cin.ignore();
    cout << "\nResponda las preguntas de recuperacion.\n";
    for (const string& pregunta : preguntas) {
        string respuesta;
        cout << pregunta << ": ";
        getline(cin, respuesta);
        archivo << email << "," << pregunta << "," << limpiarRespuesta(respuesta) << "\n";
    }
}
