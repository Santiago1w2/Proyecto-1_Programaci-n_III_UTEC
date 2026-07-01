#include "IUsuarios.h"
#include "Proxy.h"
#include "Iterator.h"

//Flujo(funciones) para la interaccion entre el programa y el usuario

static string construirListaIds(const vector<int>& ids) {
    string resultado = "[";
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i > 0) resultado += " ";
        resultado += to_string(ids[i]);
    }
    resultado += "]";
    return resultado;
}

static bool contieneId(const vector<int>& ids, int id) {
    return find(ids.begin(), ids.end(), id) != ids.end();
}

static bool pareceFechaNacimiento(const string& valor) {
    return valor.find('/') != string::npos && calcularEdad(valor) >= 0;
}

static string fechaAproximadaDesdeEdad(int edad) {
    SYSTEMTIME hoy;
    GetLocalTime(&hoy);
    int anio = hoy.wYear - max(0, edad);
    stringstream ss;
    ss << "01/01/" << anio;
    return ss.str();
}

static bool parseUsuarioLinea(const string& linea, DatosUsuario& usuario) {
    if (linea.empty()) {
        return false;
    }

    stringstream ss(linea);
    string fechaONombre;
    string nombreOLista;
    string vTarde;
    string likes;
    string baneados;
    string historial;

    getline(ss, usuario.email, ',');
    getline(ss, usuario.clave, ',');
    getline(ss, fechaONombre, ',');
    getline(ss, nombreOLista, ',');

    if (usuario.email.empty() || usuario.clave.empty() || fechaONombre.empty()) {
        return false;
    }

    if (pareceFechaNacimiento(fechaONombre)) {
        usuario.fechaNac = fechaONombre;
        usuario.nombre = nombreOLista;
        getline(ss, vTarde, ']'); vTarde += "]";
    } else {
        usuario.nombre = fechaONombre;
        int edad = 18;
        try {
            edad = stoi(nombreOLista);
        } catch (...) {
            edad = 18;
        }
        usuario.fechaNac = fechaAproximadaDesdeEdad(edad);
        getline(ss, vTarde, ']'); vTarde += "]";
    }

    if (ss.peek() == ',') ss.ignore(1);
    getline(ss, likes, ']'); likes += "]";
    if (ss.peek() == ',') ss.ignore(1);
    getline(ss, baneados, ']'); baneados += "]";
    if (ss.peek() == ',') ss.ignore(1);
    getline(ss, historial, ']'); historial += "]";

    usuario.verMasTarde = parseLista(vTarde);
    usuario.likes = parseLista(likes);
    usuario.baneados = parseLista(baneados);
    usuario.historialPeliculas = parseLista(historial);
    return true;
}

static string serializarUsuario(const DatosUsuario& usuario) {
    return usuario.email + "," + usuario.clave + "," + usuario.fechaNac + "," + usuario.nombre + "," +
           construirListaIds(usuario.verMasTarde) + "," +
           construirListaIds(usuario.likes) + "," +
           construirListaIds(usuario.baneados) + "," +
           construirListaIds(usuario.historialPeliculas);
}

static vector<DatosUsuario> leerDatosUsuarios() {
    vector<DatosUsuario> usuarios;
    ifstream archivo("registroUsuarios.txt");
    string linea;
    while (getline(archivo, linea)) {
        DatosUsuario usuario;
        if (parseUsuarioLinea(linea, usuario)) {
            usuarios.push_back(usuario);
        }
    }
    return usuarios;
}

static void guardarDatosUsuarios(const vector<DatosUsuario>& usuarios) {
    ofstream archivo("registroUsuarios.txt", ios::trunc);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de usuarios\n";
        return;
    }
    for (const DatosUsuario& usuario : usuarios) {
        archivo << serializarUsuario(usuario) << "\n";
    }
}

static string limpiarRespuestaRecuperacion(string texto) {
    texto = aMinuscula(texto);
    size_t inicio = texto.find_first_not_of(" \t\r\n");
    size_t fin = texto.find_last_not_of(" \t\r\n");
    if (inicio == string::npos) {
        return "";
    }
    return texto.substr(inicio, fin - inicio + 1);
}

static string limpiarCampoCSVSimple(string texto) {
    replace(texto.begin(), texto.end(), ',', ' ');
    replace(texto.begin(), texto.end(), '\n', ' ');
    replace(texto.begin(), texto.end(), '\r', ' ');
    return texto;
}

struct PreguntaRespuesta {
    string pregunta;
    string respuesta;
};

static vector<PreguntaRespuesta> obtenerPreguntasRecuperacion(const string& email) {
    ifstream archivo("preguntasRecuperacion.csv");
    vector<PreguntaRespuesta> preguntas;
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string correo;
        string pregunta;
        string respuesta;
        getline(ss, correo, ',');
        getline(ss, pregunta, ',');
        getline(ss, respuesta);
        if (correo == email && !pregunta.empty()) {
            preguntas.push_back({pregunta, respuesta});
        }
    }
    return preguntas;
}

static vector<string> obtenerUltimasBusquedasUsuario(const string& email, size_t limite = 5) {
    vector<string> busquedas;
    ifstream archivo("historialUsuarios.txt");
    string linea;

    while (getline(archivo, linea)) {
        if (linea.rfind(email + " | ", 0) != 0) {
            continue;
        }
        if (linea.find(" | busqueda | ") == string::npos) {
            continue;
        }

        const string marcador = " | detalle: ";
        size_t pos = linea.find(marcador);
        if (pos == string::npos) {
            continue;
        }

        string busqueda = limpiarRespuestaRecuperacion(linea.substr(pos + marcador.size()));
        if (!busqueda.empty()) {
            busquedas.push_back(busqueda);
        }
    }

    if (busquedas.size() > limite) {
        busquedas.erase(busquedas.begin(), busquedas.begin() + static_cast<ptrdiff_t>(busquedas.size() - limite));
    }
    return busquedas;
}

static bool restaurarPasswordEnRegistro(const string& email, const string& nuevaClave) {
    vector<DatosUsuario> usuarios = leerDatosUsuarios();
    bool actualizado = false;
    for (DatosUsuario& usuario : usuarios) {
        if (usuario.email == email) {
            usuario.clave = nuevaClave;
            actualizado = true;
            break;
        }
    }

    if (actualizado) {
        guardarDatosUsuarios(usuarios);
    }
    return actualizado;
}

static bool solicitarNuevaPassword(const string& email) {
    string nuevaClave;
    string repetirClave;
    cout << "\nNueva contrasena: ";
    cin >> nuevaClave;
    cout << "Repetir nueva contrasena: ";
    cin >> repetirClave;

    if (nuevaClave.empty() || nuevaClave != repetirClave) {
        limpiarPantalla();
        cout << "\nLas contrasenas no coinciden.\n";
        esperarEnter();
        limpiarPantalla();
        return false;
    }

    if (!restaurarPasswordEnRegistro(email, nuevaClave)) {
        limpiarPantalla();
        cout << "\nNo se encontro el usuario.\n";
        esperarEnter();
        limpiarPantalla();
        return false;
    }

    limpiarPantalla();
    cout << "\nContrasena actualizada correctamente.\n";
    esperarEnter();
    limpiarPantalla();
    return true;
}

static bool restaurarPasswordConHistorial(const string& email) {
    vector<string> ultimasBusquedas = obtenerUltimasBusquedasUsuario(email);
    if (ultimasBusquedas.size() < 5) {
        limpiarPantalla();
        cout << "\nNo hay suficientes busquedas recientes para recuperar por historial.\n";
        esperarEnter();
        limpiarPantalla();
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nIngresa 2 busquedas de tus ultimas 5 busquedas realizadas.\n";
    string respuesta1;
    string respuesta2;
    cout << "Busqueda 1: ";
    getline(cin, respuesta1);
    cout << "Busqueda 2: ";
    getline(cin, respuesta2);

    unordered_set<string> respuestas = {
        limpiarRespuestaRecuperacion(respuesta1),
        limpiarRespuestaRecuperacion(respuesta2)
    };

    int coincidencias = 0;
    for (const string& busqueda : ultimasBusquedas) {
        if (respuestas.count(busqueda)) {
            coincidencias++;
        }
    }

    if (coincidencias < 2) {
        limpiarPantalla();
        cout << "\nNo coinciden suficientes busquedas.\n";
        esperarEnter();
        limpiarPantalla();
        return false;
    }

    return solicitarNuevaPassword(email);
}

vector<int> parseLista(const string& s) {
    vector<int> res;
    string num = "";
    for (char c : s) {
        if (isdigit(c)) {
            num += c;
        } else {
            if (!num.empty()) {
                try { res.push_back(stoi(num)); } catch (...) {}
                num = "";
            }
        }
    }
    if (!num.empty()) {
        try { res.push_back(stoi(num)); } catch (...) {}
    }
    return res;
}

//Se utiliza para las peliculas de los usuarios (Likes,Ban,etc)
//Busca los 'ids' registrados y los guarda en un mapa
unordered_map<int, Movie> convertirAPelis(const vector<int>& ids, const unordered_map<int, Movie>& pelis) {
    unordered_map<int, Movie> res;
    for (int id : ids) {
        auto it = pelis.find(id);
        if (it != pelis.end()) {
            res.insert({id, it->second});
        }
    }
    return res;
}

vector<Usuario> leerUsuarios(const string &csv, const unordered_map<int,Movie>& pelis) {
    vector<Usuario> resultado;
    ifstream archivo(csv);
    if (!archivo.is_open()) {
        cout << "ERROR: no se pudo abrir el archivo de usuarios\n";
        system("pause");
        exit(1);
    }
    string linea;
    getline(archivo, linea);
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string _user, _fechaNac, _email, _pass, _vTarde, _likes, _ban, _hist;

        getline(ss, _email, ',');
        getline(ss, _pass, ',');
        getline(ss, _fechaNac, ',');
        getline(ss, _user, ',');

        getline(ss, _vTarde, ']'); _vTarde += "]"; ss.ignore(1);
        getline(ss, _likes, ']'); _likes += "]"; ss.ignore(1);
        getline(ss, _ban, ']'); _ban += "]"; ss.ignore(1);
        getline(ss, _hist, ']'); _hist += "]";

        unordered_map<int,Movie> VT = convertirAPelis(parseLista(_vTarde), pelis);
        unordered_map<int,Movie> MG = convertirAPelis(parseLista(_likes), pelis);
        unordered_map<int,Movie> Ban = convertirAPelis(parseLista(_ban), pelis);
        unordered_map<int,Movie> Hist = convertirAPelis(parseLista(_hist), pelis);

        Usuario us(_user, _email, _fechaNac, _pass, VT, MG, Ban, Hist);
        resultado.push_back(us);
    }
    archivo.close();
    return resultado;
}


//Se utiliza cuando un usuario nuevo se va a registrar
//Verifica que el correo que esta intentando ingresar no esta registrado
//Si lo tomamos como una tabla en una base de datos, el correo es la llave primaria
bool validar_correo(const string& _email) {
    for (const DatosUsuario& usuario : leerDatosUsuarios()) {
        if (usuario.email == _email) return true;
    }
    return false;
}

//Se utiliza para usuarios que ya estan registrados
//Valida que las credenciales que estan ingresando sean iguales a las registradas
bool validar_info(const string& _email, const string& _clave) {
    DatosUsuario usuario;
    return obtener_usuario(_email, _clave, usuario);
}

bool obtener_usuario(const string& email, const string& clave, DatosUsuario& usuario) {
    for (const DatosUsuario& actual : leerDatosUsuarios()) {
        if (actual.email == email && actual.clave == clave) {
            usuario = actual;
            return true;
        }
    }
    return false;
}

bool obtener_usuario_por_email(const string& email, DatosUsuario& usuario) {
    for (const DatosUsuario& actual : leerDatosUsuarios()) {
        if (actual.email == email) {
            usuario = actual;
            return true;
        }
    }
    return false;
}


//Se utiliza para obtener el nombre del usuario correspondiente a las credenciales ingresadas (cuando ya se ha validado)
string UserName(const string& _email, const string& _clave) {
    DatosUsuario usuario;
    if (obtener_usuario(_email, _clave, usuario)) {
        return usuario.nombre;
    }
    return "";
}

int UserEdad(const string& _email, const string& _clave) {
    DatosUsuario usuario;
    if (obtener_usuario(_email, _clave, usuario)) {
        int edad = calcularEdad(usuario.fechaNac);
        return edad < 0 ? 18 : edad;
    }
    return 18;
}

//Luego de realizar la validación del nuevo usuario (correo no repetido), se registra la información en el historial de usuarios.
string UserFechaNac(const string& _email, const string& _clave) {
    DatosUsuario usuario;
    if (obtener_usuario(_email, _clave, usuario)) {
        return usuario.fechaNac;
    }
    return "01/01/2000";
}

void registrar_nuevoUsuario(const string& name, const string& fechaNac, const string& email, const string& clave) {
    ofstream archivo("registroUsuarios.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de usuarios\n";
        return;
    }
    archivo << email << "," << clave << "," << fechaNac << "," << name << "," << "[],[],[],[]\n";
    archivo.close();
}

void registrarPreguntasRecuperacion(const string& email) {
    if (email.empty()) {
        return;
    }

    if (!obtenerPreguntasRecuperacion(email).empty()) {
        return;
    }

    cout << "\nDesea activar preguntas de recuperacion? (S/N): ";
    char opcion;
    cin >> opcion;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (opcion != 'S' && opcion != 's') {
        return;
    }

    const vector<string> preguntas = {
        "Numero de celular",
        "Primer nombre de su padre",
        "Primer nombre de su madre",
        "Gaseosa favorita",
        "Videojuego favorito",
        "Deporte favorito",
        "Comida favorita"
    };

    ofstream archivo("preguntasRecuperacion.csv", ios::app);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir preguntasRecuperacion.csv\n";
        return;
    }

    cout << "\nResponde las preguntas de recuperacion.\n";
    for (const string& pregunta : preguntas) {
        string respuesta;
        cout << pregunta << ": ";
        getline(cin, respuesta);
        archivo << email << ","
                << limpiarCampoCSVSimple(pregunta) << ","
                << limpiarCampoCSVSimple(limpiarRespuestaRecuperacion(respuesta)) << "\n";
    }
}

bool restaurarPasswordConPreguntas(const string& email) {
    vector<PreguntaRespuesta> preguntas = obtenerPreguntasRecuperacion(email);
    if (preguntas.size() < 3) {
        limpiarPantalla();
        cout << "\nEste usuario no tiene preguntas de recuperacion suficientes.\n";
        cout << "Se intentara recuperar por historial de busquedas.\n";
        esperarEnter();
        limpiarPantalla();
        return restaurarPasswordConHistorial(email);
    }

    shuffle(preguntas.begin(), preguntas.end(), mt19937(random_device{}()));
    const int cantidad = min(3, static_cast<int>(preguntas.size()));
    int correctas = 0;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nResponde las preguntas para restaurar tu contrasena.\n";
    for (int i = 0; i < cantidad; ++i) {
        string respuesta;
        cout << preguntas[i].pregunta << ": ";
        getline(cin, respuesta);
        if (limpiarRespuestaRecuperacion(respuesta) == limpiarRespuestaRecuperacion(preguntas[i].respuesta)) {
            correctas++;
        }
    }

    if (correctas != cantidad) {
        limpiarPantalla();
        cout << "\nLas respuestas no coinciden.\n";
        esperarEnter();
        limpiarPantalla();
        return false;
    }

    return solicitarNuevaPassword(email);
}

static bool agregarIdEnCampoUsuario(const string& email, int movieId, const string& campo) {
    vector<DatosUsuario> usuarios = leerDatosUsuarios();
    bool actualizado = false;

    for (DatosUsuario& usuario : usuarios) {
        if (usuario.email != email) {
            continue;
        }

        vector<int>* destino = nullptr;
        if (campo == "likes") {
            destino = &usuario.likes;
        } else if (campo == "verMasTarde") {
            destino = &usuario.verMasTarde;
        } else if (campo == "historial") {
            destino = &usuario.historialPeliculas;
        }

        if (destino != nullptr && !contieneId(*destino, movieId)) {
            destino->push_back(movieId);
        }
        actualizado = true;
        break;
    }

    if (actualizado) {
        guardarDatosUsuarios(usuarios);
    }
    return actualizado;
}

bool agregarLikeUsuario(const string& email, int movieId) {
    return agregarIdEnCampoUsuario(email, movieId, "likes");
}

bool agregarVerMasTardeUsuario(const string& email, int movieId) {
    return agregarIdEnCampoUsuario(email, movieId, "verMasTarde");
}

bool agregarFavoritoUsuario(const string& email, int movieId) {
    return agregarIdEnCampoUsuario(email, movieId, "likes");
}

static bool actualizarUsuario(const string& email, const function<void(DatosUsuario&)>& cambio) {
    vector<DatosUsuario> usuarios = leerDatosUsuarios();
    bool actualizado = false;
    for (DatosUsuario& usuario : usuarios) {
        if (usuario.email == email) {
            cambio(usuario);
            actualizado = true;
            break;
        }
    }
    if (actualizado) {
        guardarDatosUsuarios(usuarios);
    }
    return actualizado;
}

bool actualizarNombreUsuario(const string& email, const string& nuevoNombre) {
    if (nuevoNombre.empty()) {
        return false;
    }
    return actualizarUsuario(email, [&](DatosUsuario& usuario) {
        usuario.nombre = nuevoNombre;
    });
}

bool actualizarFechaNacimientoUsuario(const string& email, const string& nuevaFechaNac) {
    if (calcularEdad(nuevaFechaNac) < 0) {
        return false;
    }
    return actualizarUsuario(email, [&](DatosUsuario& usuario) {
        usuario.fechaNac = nuevaFechaNac;
    });
}

bool actualizarPasswordUsuario(const string& email, const string& nuevaPassword) {
    if (nuevaPassword.empty()) {
        return false;
    }
    return actualizarUsuario(email, [&](DatosUsuario& usuario) {
        usuario.clave = nuevaPassword;
    });
}

void registrarHistorialUsuario(const string& email, const string& tipo, int movieId, const string& detalle) {
    ofstream archivo("historialUsuarios.txt", ios::app);
    if (!archivo.is_open()) {
        return;
    }

    HistorialEntry entry(movieId, tipo, detalle);
    archivo << email << " | " << entry.timestamp
            << " | " << tipo
            << " | id: " << movieId
            << " | detalle: " << detalle << "\n";

    if (movieId > 0 && tipo == "visualizacion") {
        agregarIdEnCampoUsuario(email, movieId, "historial");
    }
}

vector<HistorialEntry> leerHistorialUsuario(const string& email) {
    vector<HistorialEntry> historial;
    ifstream archivo("historialUsuarios.txt");
    string linea;
    while (getline(archivo, linea)) {
        if (linea.rfind(email + " | ", 0) != 0) {
            continue;
        }

        vector<string> partes;
        string parte;
        stringstream ss(linea);
        while (getline(ss, parte, '|')) {
            while (!parte.empty() && parte.front() == ' ') parte.erase(parte.begin());
            while (!parte.empty() && parte.back() == ' ') parte.pop_back();
            partes.push_back(parte);
        }
        if (partes.size() < 5) {
            continue;
        }

        int movieId = -1;
        if (partes[3].rfind("id: ", 0) == 0) {
            try {
                movieId = stoi(partes[3].substr(4));
            } catch (...) {
                movieId = -1;
            }
        }

        string detalle = partes[4];
        if (detalle.rfind("detalle: ", 0) == 0) {
            detalle = detalle.substr(9);
        }

        HistorialEntry entry(movieId, partes[2], detalle);
        entry.timestamp = partes[1];
        historial.push_back(entry);
    }
    return historial;
}

void mostrarHistorialUsuario(const string& email) {
    limpiarPantalla();
    vector<HistorialEntry> historial = leerHistorialUsuario(email);
    HistorialIterator iterador(historial);

    cout << "===== HISTORIAL DE ACCIONES =====\n\n";
    if (iterador.estaVacio()) {
        cout << "Aun no hay acciones registradas.\n";
        esperarEnter();
        return;
    }

    while (iterador.haySiguiente()) {
        HistorialEntry entry = iterador.siguiente();
        cout << entry.timestamp << " | " << entry.tipo
             << " | id: " << entry.movieId
             << " | " << entry.detalle << "\n";
    }
    esperarEnter();
}

void mostrarFavoritosUsuario(const string& email, const unordered_map<int, Movie>& pelis) {
    limpiarPantalla();
    DatosUsuario usuario;
    cout << "===== FAVORITOS =====\n\n";
    if (!obtener_usuario_por_email(email, usuario) || usuario.likes.empty()) {
        cout << "Aun no hay peliculas favoritas.\n";
        esperarEnter();
        return;
    }

    for (int id : usuario.likes) {
        auto it = pelis.find(id);
        if (it == pelis.end()) {
            continue;
        }
        cout << "[" << id << "] " << it->second.getTtitle()
             << " | " << it->second.getGenre()
             << " | " << it->second.getYear() << "\n";
    }
    esperarEnter();
}


//Fncion utilizada par visualizar a los usuarios registrados (seria adecuado para un rol admin)
vector<string> mostrar_usuarios() {
    ifstream archivo("registroUsuarios.txt");
    string linea, username, email, clave;
    vector<string> usuarios_name;
    if (!archivo.is_open()) return {};

    bool hayDatos = false;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');
        usuarios_name.push_back(username);
        hayDatos = true;
    }
    if (!hayDatos) cout << "No hay usuarios registrados\n";
    archivo.close();
    return usuarios_name;
}



//Recomendacion aleatoria de 5 peliculas
void peliculasRecomendadasPanel(const unordered_map<int, Movie>& pelis, int edadUsuario) {
    //Historial temporal de peliculas que se estan recomendando en una misma interacción
    unordered_map<int,string> hist_temp;

    random_device rd;
    mt19937 gen(rd());
    vector<int> ids;
    ids.reserve(pelis.size());
    for (const auto& [idPelicula, pelicula] : pelis) {
        ids.push_back(idPelicula);
    }
    int n=ids.size();
    if (n==0) return; //Queremos que la funcion sea lo mas escalable posible
    uniform_int_distribution<> dist(0, n - 1);
    int id;
    string genre;

    // Algoritmo para la recomendacion aletoria de peliculas
    for (int i=0;i<5 && i<n;i++) {
        bool cond1 = false;
        bool cond2 = false; //Es necesario que se inicialice como false ya que no se aplica para la primera pelicula (i=0)
        bool cond3 = false;
        int intentos = 0;
        do {
            id = ids[dist(gen)];

            //No queremos que se recomiende una pelicula más de una vez en una misma interaccion.
            //Se verifica que el ID de la pelicula no este registrado en el historial
            cond1 = hist_temp.find(id)==hist_temp.end();

            //Se busca recomendar peliculas de diferente genero.
            auto it = pelis.find(id);
            if (it == pelis.end()) {
                cond1 = false;
                intentos++;
                continue;
            }
            PeliculaReal real(&it->second);
            PeliculaProxy proxy(&real, edadUsuario);
            cond3 = !proxy.puedeMostrarse();
            genre = it->second.getGenre();
            cond2 = false;
            if (i != 0)
                for (auto& e:hist_temp)
                    if (e.second==genre) {
                        cond2 = true;
                        break;
                    }
            //COND1 -> TRUE: ID aun no esta registrado
            //COND2 -> TRUE: Ya se ha registrado una pelicula con el mismo genero
            intentos++;
        } while ((cond1==false || cond2==true || cond3==true) && intentos < 100);
        if (!cond1 || cond2 || cond3) continue;
        hist_temp[id] = genre;
    }

    // Algoritmo para la impresion de las recomendaciones
    int fila = 8;
    int col = 45;
    for (auto& k:hist_temp) {
        const Movie& m = pelis.at(k.first);
        PeliculaReal real(&m);
        PeliculaProxy proxy(&real, edadUsuario);
        string titulo = proxy.getTitulo();
        if (titulo.size() > 25)
            titulo = titulo.substr(0, 25);
        moverCursor(col, fila);
        cout << "[" << k.first << "] " << titulo;
        cout << " " << proxy.getGenero() << " | " << proxy.getAnio();
        fila++;
    }
}
