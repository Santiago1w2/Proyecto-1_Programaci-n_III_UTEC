#include "Interfaz.h"

void moverCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void limpiarPantalla() {system("cls");}

void esperarEnter() {
    cout << "\nPresiona Enter para continuar...";
    if (cin.peek() == '\n') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.get(); // espera Enter real
}
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Home() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul"); // activar UTF-8
    setColor(11); // Cyan (celeste)
    cout << R"(
                                 ██╗   ██╗████████╗███████╗ ██████╗███████╗██╗     ██╗██╗  ██╗
                                 ██║   ██║╚══██╔══╝██╔════╝██╔════╝██╔════╝██║     ██║╚██╗██╔╝
                                 ██║   ██║   ██║   █████╗  ██║     █████╗  ██║     ██║ ╚███╔╝
                                 ██║   ██║   ██║   ██╔══╝  ██║     ██╔══╝  ██║     ██║ ██╔██╗
                                 ╚██████╔╝   ██║   ███████╗╚██████╗██║     ███████╗██║██╔╝ ██╗
                                  ╚═════╝    ╚═╝   ╚══════╝ ╚═════╝╚═╝     ╚══════╝╚═╝╚═╝  ╚═╝
)" << endl;

    setColor(14); // Amarillo

    cout << "\n                                               🎬 Películas y series\n";

    setColor(10); // Verde

    cout << "                                                  sin límites y mucho más 🍿\n";

    setColor(7); // Blanco normal (reset)
    esperarEnter();
    limpiarPantalla();

}


void mostrarMenu(int opcion) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    system("cls");

    cout << R"(
╭────────────────────────────────────╮
│        * INGRESO AL SISTEMA *      │
├────────────────────────────────────┤
)";

    // OPCIÓN A
    if (opcion == 1) setColor(11), cout << "│   -> Iniciar sesión                │\n";
    else setColor(7), cout << "│      Iniciar sesión                │\n";

    // OPCIÓN B
    if (opcion == 2) setColor(11), cout << "│   -> Registrarse                   │";
    else setColor(7), cout << "│      Registrarse                   │";

    setColor(7);

    cout << R"(
├────────────────────────────────────┤
│   Usa ↑ ↓ y ENTER                  │
╰────────────────────────────────────╯
)";
}

void seleccionar_opcion(char& op) {
    int opcion = 1;
    char tecla;

    while (true) {
        mostrarMenu(opcion);

        tecla = getch(); // lee tecla sin ENTER

        if (tecla == 72) { // ↑
            opcion--;
            if (opcion < 1) opcion = 2;
        }

        if (tecla == 80) { // ↓
            opcion++;
            if (opcion > 2) opcion = 1;
        }

        if (tecla == 13) { // ENTER
            system("cls");

            if (opcion == 1) {
                op = 'a';
            } else if (opcion == 2) {
                op = 'b';
            }

            break;
        }
    }

}

void mostrar_inicio_sesion() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    cout << R"(
╭────────────────────────────────────────────────────╮
|                 *INICIO DE SESION*                 |
├────────────────────────────────────────────────────┤
|Email:                                              |
├────────────────────────────────────────────────────┤
|Password:                                           |
╰────────────────────────────────────────────────────╯
                    ╭────────────╮
                    │  Ingresar  │
                    ╰────────────╯
)";
}

void inicio_sesion(string& correo, string& pass) {
    mostrar_inicio_sesion();
    moverCursor(7,4);
    cin >> correo;
    moverCursor(10, 6);
    cin >> pass;
}
void esperar(int n) {
    this_thread::sleep_for(chrono::seconds(n));
}
void mostrar_registro_usuario() {
    cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|User Name:                                          |
├────────────────────────────────────────────────────┤
|Email:                                              |
├────────────────────────────────────────────────────┤
|Contraseña:                                         |
├────────────────────────────────────────────────────┤
|Repetir contraseña:                                 |
╰────────────────────────────────────────────────────╯
                    ╭─────────────╮
                    │  Registrar  │
                    ╰─────────────╯
)";
}

static string construirFechaNacimiento(int dia, int mes, int anio) {
    stringstream ss;
    ss << setw(2) << setfill('0') << dia << "/"
       << setw(2) << setfill('0') << mes << "/"
       << setw(4) << setfill('0') << anio;
    return ss.str();
}

static string pedirFechaNacimiento() {
    int dia, mes, anio;
    string fecha;
    do {
        cout << "Dia de nacimiento: ";
        cin >> dia;
        cout << "Mes de nacimiento: ";
        cin >> mes;
        cout << "Anio de nacimiento: ";
        cin >> anio;
        fecha = construirFechaNacimiento(dia, mes, anio);
        if (calcularEdad(fecha) < 0) {
            cout << "Fecha invalida. Intenta nuevamente.\n";
        }
    } while (calcularEdad(fecha) < 0);
    return fecha;
}

void registro(string& correo, string& pass, string& name, string& clave, string& fechaNac) {
    mostrar_registro_usuario();
    moverCursor(12,4);
    cin >> name;
    moverCursor(7, 6);
    cin >> correo;
    moverCursor(12, 8);
    cin >> pass;
    moverCursor(21, 10);
    cin >> clave;
    moverCursor(1, 14);
    fechaNac = pedirFechaNacimiento();
}

void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, string& us_fechaNac, char& opcion_entrada) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    if (opcion_entrada=='a') {
        inicio_sesion(us_email,us_password);
        while (!validar_info(us_email,us_password)) {
            limpiarPantalla();
            cout << R"(
╭───────────────────────────────╮
│ ⚠ ERROR EN LAS CREDENCIALES   │
╰───────────────────────────────╯
)";
            esperar(1);
            limpiarPantalla();
            cout << "Escribe R para restaurar contrasena o cualquier otra tecla para reintentar: ";
            char opcionRecuperacion;
            cin >> opcionRecuperacion;
            if (opcionRecuperacion == 'R' || opcionRecuperacion == 'r') {
                restaurarPasswordConPreguntas(us_email);
            }
            limpiarPantalla();
            inicio_sesion(us_email,us_password);
        }
        limpiarPantalla();
        us_name=UserName(us_email,us_password);
        us_fechaNac=UserFechaNac(us_email,us_password);
        cout << R"(
╭───────────────────────────────╮
│  ✔ INICIO DE SESION EXITOSO   │
╰───────────────────────────────╯
)"; esperar(1);
        limpiarPantalla();
    }
    else {
        string clave_temp;
        registro(us_email,us_password,us_name,clave_temp,us_fechaNac);
        while (validar_correo(us_email)) {
            limpiarPantalla();
            cout << R"(
╭───────────────────────────────╮
│    ⚠ CORREO YA REGISTRADO     │
╰───────────────────────────────╯
)"; esperar(2);
            limpiarPantalla();
            registro(us_email,us_password,us_name, clave_temp,us_fechaNac);
        }
        while (us_password!=clave_temp) {
            limpiarPantalla();
            cout << R"(
╭─────────────────────────────────────────────────────╮
│    ⚠ LAS CLAVES NO COINCIDEN. VUELVA A INTENTAR     │
╰─────────────────────────────────────────────────────╯
)";
            esperar(2);
            limpiarPantalla();
            registro(us_email,us_password,us_name, clave_temp,us_fechaNac);
        }
        limpiarPantalla();
        cout << R"(
╭───────────────────────────────────╮
│  ✔ USUARIO REGISTRADO CON EXITO   │
╰───────────────────────────────────╯
)";
        esperar(3);

        limpiarPantalla();
        registrar_nuevoUsuario(us_name,us_fechaNac,us_email,us_password);
        registrarPreguntasRecuperacion(us_email);
    }
}

void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, char& n, int edadUsuario) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
    system("cls");
    // ===== INTERFAZ =====
    cout << "╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║ 🎬 UTECFLIX                                                                                                                                   ║ \n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n";

    moverCursor(110,1);
    cout << "Usuario: " << nombre << "\n\n";


    cout << "╭─────────────── MENÚ ───────────────╮   ╭──────────────────── CONTENIDO ───────────────────────────────────────────────────────────────────────╮\n";
    cout << "│                                    │   │   🎥 RECOMENDACIONES                                                                                 │\n";
    cout << "│  A. 👤 Mi Perfil                   │   │   ───────────────────────────────                                                                    │\n";
    cout << "│  B. 🕘 Historial                   │   │                                                                                                      │\n";
    cout << "│  C. ⭐ Favoritos                    │   │                                                                                                      │\n";
    cout << "│  D. 🔍 Buscar                      │   │                                                                                                      │\n";
    cout << "│                                    │   │                                                                                                      │\n";
    cout << "│  0. Salir                          │   │                                                                                                      │\n";
    cout << "│                                    │   │                                                                                                      │\n";
    cout << "│                                    │   │                                                                                                      │\n";
    cout << "│                                    │   │                                                                                                      │\n";
    cout << "╰────────────────────────────────────╯   ╰──────────────────────────────────────────────────────────────────────────────────────────────────────╯\n";

    // ===== MOSTRAR PELÍCULAS EN PANEL DERECHO =====
    peliculasRecomendadasPanel(pelis, edadUsuario);

    // ===== INPUT =====
    moverCursor(0, 20);
    cout << "👉 Selecciona una opción: ";
    cin >> n;
}

static bool validarPasswordActual(const string& email) {
    string passwordActual;
    cout << "Confirma tu contrasena actual: ";
    cin >> passwordActual;
    DatosUsuario usuario;
    return obtener_usuario(email, passwordActual, usuario);
}

void mostrarPerfilUsuario(string& email, string& password, string& nombre, string& fechaNac, int& edad) {
    while (true) {
        limpiarPantalla();
        DatosUsuario usuario;
        obtener_usuario_por_email(email, usuario);

        nombre = usuario.nombre.empty() ? nombre : usuario.nombre;
        fechaNac = usuario.fechaNac.empty() ? fechaNac : usuario.fechaNac;
        edad = calcularEdad(fechaNac);
        if (edad < 0) edad = 18;

        cout << "===== PERFIL DE USUARIO =====\n\n";
        cout << "Nombre de usuario: " << nombre << "\n";
        cout << "Fecha de nacimiento: " << fechaNac << "\n";
        cout << "Edad: " << edad << "\n";
        cout << "Correo: " << email << "\n";
        cout << "\nOpciones:\n";
        cout << "1. Cambiar nombre de usuario\n";
        cout << "2. Cambiar fecha de nacimiento\n";
        cout << "3. Cambiar contrasena\n";
        cout << "0. Volver\n";
        cout << "Selecciona una opcion: ";

        char opcion;
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == '0') {
            limpiarPantalla();
            return;
        }

        if (!validarPasswordActual(email)) {
            cout << "\nContrasena incorrecta. No se realizaron cambios.\n";
            esperarEnter();
            continue;
        }

        unique_ptr<PerfilCommand> comando;
        if (opcion == '1') {
            string nuevoNombre;
            cout << "Nuevo nombre: ";
            cin >> nuevoNombre;
            comando = make_unique<CambiarNombreCommand>(email, nuevoNombre);
            nombre = nuevoNombre;
        } else if (opcion == '2') {
            string nuevaFechaNac = pedirFechaNacimiento();
            comando = make_unique<CambiarFechaNacimientoCommand>(email, nuevaFechaNac);
            fechaNac = nuevaFechaNac;
            edad = calcularEdad(fechaNac);
            if (edad < 0) edad = 18;
        } else if (opcion == '3') {
            string nuevaPassword;
            string repetirPassword;
            cout << "Nueva contrasena: ";
            cin >> nuevaPassword;
            cout << "Repetir nueva contrasena: ";
            cin >> repetirPassword;
            if (nuevaPassword != repetirPassword) {
                cout << "\nLas contrasenas no coinciden.\n";
                esperarEnter();
                continue;
            }
            comando = make_unique<CambiarPasswordCommand>(email, nuevaPassword);
            password = nuevaPassword;
        }

        if (comando && comando->ejecutar()) {
            registrarHistorialUsuario(email, "perfil", -1, "datos actualizados");
            cout << "\nCambio aplicado correctamente.\n";
        } else {
            cout << "\nNo se pudo aplicar el cambio.\n";
        }
        esperarEnter();
    }
}

void interfaz_resultado() {
    cout << "╔═══════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║ Buscar:                                                                                               ║ \n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════════════════════════════════╝\n";
    cout << "╭────────────────────  RESULTADOS ─────────────────────────────────────────────────────────────────────╮\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "│                                                                                                      │\n";
    cout << "╰──────────────────────────────────────────────────────────────────────────────────────────────────────╯\n";

}
void seleccion_pelicula(int id_peli, unordered_map<int, Movie>& pelis, const string& emailUsuario, int edadUsuario);

void interfaz_buscar(unordered_map<int, Movie>& pelis, Procesador& pre_procesador, const string& emailUsuario, int edadUsuario) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
    system("cls");
    // ===== INTERFAZ =====
    string consulta;
    char opBusqueda;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while(true)
    {
        interfaz_resultado();

        moverCursor(10,1);
        getline(cin, consulta);
        if (!consulta.empty()) {
            registrarHistorialUsuario(emailUsuario, "busqueda", -1, consulta);
        }

        vector<int> resultados = pre_procesador.buscar(consulta);
        ResultadosIterator iterador(resultados, 5);

        if(iterador.estaVacio())
        {
            moverCursor(8,3);
            cout << "Sin resultados";

            moverCursor(1,14);
            cout << R"(
Selecciona una opción:
 - X: Salir
 - V: Volver a buscar )";

            cin >> opBusqueda;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if(opBusqueda == 'X')
                return;

            limpiarPantalla();
            continue;
        }

        bool nuevaBusqueda = false;
        while (!nuevaBusqueda) {
        moverCursor(6, 3);
        cout << "Pagina " << iterador.paginaActualNumero() << "/" << iterador.totalPaginas()
             << " | Resultados: " << iterador.totalResultados();

        int y = 8;
        vector<int> pagina = iterador.paginaActual();

        for(int id : pagina)
        {
            moverCursor(6, y++);
            auto it = pelis.find(id);
            if (it == pelis.end()) continue;
            PeliculaReal real(&it->second);
            PeliculaProxy proxy(&real, edadUsuario);
            cout << id << " -> " << proxy.getTitulo() << endl;
        }

        moverCursor(1,14);
        cout << R"(
Selecciona una opción:
 - X: Salir
 - V: Volver a buscar
 - S: Siguiente pagina
 - A: Pagina anterior
 - C: Selecionar una pelicula
)";

        cin >> opBusqueda;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        opBusqueda = static_cast<char>(toupper(static_cast<unsigned char>(opBusqueda)));
        if(opBusqueda == 'X')
            return;
        if(opBusqueda == 'V') {
            nuevaBusqueda = true;
            limpiarPantalla();
            continue;
        }
        if(opBusqueda == 'S') {
            iterador.siguientePagina();
        }
        if(opBusqueda == 'A') {
            iterador.paginaAnterior();
        }
        if(opBusqueda == 'C') {
            int idSeleccionado;
            cout << "\nID de pelicula: ";
            cin >> idSeleccionado;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            seleccion_pelicula(idSeleccionado, pelis, emailUsuario, edadUsuario);
        }
        limpiarPantalla();
        interfaz_resultado();
        moverCursor(10,1);
        cout << consulta;
        }


    }
}
void seleccion_pelicula(int id_peli, unordered_map<int, Movie>& pelis, const string& emailUsuario, int edadUsuario) {
    auto it = pelis.find(id_peli);
    limpiarPantalla();
    if (it == pelis.end()) {
        cout << "No existe una pelicula con ese ID.\n";
        esperarEnter();
        return;
    }

    PeliculaReal real(&it->second);
    PeliculaProxy proxy(&real, edadUsuario);

    cout << "ID: " << id_peli << "\n";
    cout << "Titulo: " << proxy.getTitulo() << "\n";
    cout << "Genero: " << proxy.getGenero() << "\n";
    cout << "Anio: " << proxy.getAnio() << "\n";
    if (proxy.puedeMostrarse()) {
        registrarHistorialUsuario(emailUsuario, "visualizacion", id_peli, it->second.getTtitle());
        cout << "Director: " << it->second.getDirector() << "\n";
        cout << "Cast: " << it->second.getCast() << "\n";
        cout << "Sinopsis: " << it->second.getPlot() << "\n";
    } else {
        cout << "Esta pelicula esta restringida para el usuario actual.\n";
        esperarEnter();
        return;
    }

    cout << "\nOpciones:\n";
    cout << " - L: Like\n";
    cout << " - V: Ver mas tarde\n";
    cout << " - F: Anadir a favoritos\n";
    cout << " - X: Volver\n";
    cout << "Selecciona una opcion: ";

    char opcion;
    cin >> opcion;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    opcion = static_cast<char>(toupper(static_cast<unsigned char>(opcion)));

    NotificacionesArchivoObserver observador;
    AccionUsuarioSubject subject;
    subject.agregarObservador(&observador);

    if (opcion == 'L') {
        agregarLikeUsuario(emailUsuario, id_peli);
        registrarHistorialUsuario(emailUsuario, "like", id_peli, it->second.getTtitle());
        subject.notificar({"LIKE", id_peli, it->second.getTtitle()});
        cout << "\nLike registrado.\n";
    } else if (opcion == 'V') {
        agregarVerMasTardeUsuario(emailUsuario, id_peli);
        registrarHistorialUsuario(emailUsuario, "ver_mas_tarde", id_peli, it->second.getTtitle());
        subject.notificar({"VER_MAS_TARDE", id_peli, it->second.getTtitle()});
        cout << "\nPelicula agregada a Ver mas tarde.\n";
    } else if (opcion == 'F') {
        agregarFavoritoUsuario(emailUsuario, id_peli);
        registrarHistorialUsuario(emailUsuario, "favorito", id_peli, it->second.getTtitle());
        subject.notificar({"FAVORITO", id_peli, it->second.getTtitle()});
        cout << "\nPelicula agregada a favoritos.\n";
    }
    esperarEnter();
}
