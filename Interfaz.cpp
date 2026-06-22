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
    cin.get(); // espera Enter real
}
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void inicio() {
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

void registro(string& correo, string& pass, string& name, string& clave) {
    mostrar_registro_usuario();
    moverCursor(12,4);
    cin >> name;
    moverCursor(7, 6);
    cin >> correo;
    moverCursor(12, 8);
    cin >> pass;
    moverCursor(21, 10);
    cin >> clave;
}

void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, char& opcion_entrada) {
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
            inicio_sesion(us_email,us_password);
        }
        limpiarPantalla();
        us_name=UserName(us_email,us_password);
        cout << R"(
╭───────────────────────────────╮
│  ✔ INICIO DE SESION EXITOSO   │
╰───────────────────────────────╯
)"; esperar(1);
        limpiarPantalla();
    }
    else {
        string clave_temp;
        registro(us_email,us_password,us_name,clave_temp);
        while (validar_correo(us_email)) {
            limpiarPantalla();
            cout << R"(
╭───────────────────────────────╮
│    ⚠ CORREO YA REGISTRADO     │
╰───────────────────────────────╯
)"; esperar(2);
            limpiarPantalla();
            registro(us_email,us_password,us_name, clave_temp);
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
            registro(us_email,us_password,us_name, clave_temp);
        }
        limpiarPantalla();
        cout << R"(
╭───────────────────────────────────╮
│  ✔ USUARIO REGISTRADO CON EXITO   │
╰───────────────────────────────────╯
)";
        esperar(3);

        limpiarPantalla();
        registrar_nuevoUsuario(us_name,us_email,us_password);
    }
}

void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, char& n) {
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
    peliculasRecomendadasPanel(pelis);

    // ===== INPUT =====
    moverCursor(0, 20);
    cout << "👉 Selecciona una opción: ";
    cin >> n;
}

void mostrarHistorial(Usuario& usuario, const unordered_map<int, Movie>& pelis) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");

    char opcion;
    do {
        limpiarPantalla();
        cout << "╔═══════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║                        🕘 HISTORIAL                                      ║\n";
        cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";

        cout << "  [1] Ver historial de películas\n";
        cout << "  [2] Ver historial de búsquedas\n";
        cout << "  [3] Ver todo el historial\n";
        cout << "  [4] Guardar snapshot del historial\n";
        cout << "  [5] Restaurar un snapshot\n";
        cout << "  [0] Volver\n\n";
        cout << "  👉 Selecciona una opción: ";
        cin >> opcion;

        if (opcion == '1') {
            limpiarPantalla();
            cout << "═══ HISTORIAL DE PELÍCULAS ═══\n\n";
            HistorialIterator it = usuario.getHistorialIteratorFiltrado("pelicula");
            if (it.estaVacio()) {
                cout << "  No hay películas en el historial.\n";
            } else {
                int idx = 1;
                while (it.haySiguiente()) {
                    HistorialEntry entry = it.siguiente();
                    cout << "  " << idx << ". [" << entry.movieId << "]";
                    auto itPeli = pelis.find(entry.movieId);
                    if (itPeli != pelis.end())
                        cout << " " << itPeli->second.getTtitle();
                    cout << "  (" << entry.timestamp << ")\n";
                    idx++;
                }
            }
            esperarEnter();
        }
        else if (opcion == '2') {
            limpiarPantalla();
            cout << "═══ HISTORIAL DE BÚSQUEDAS ═══\n\n";
            HistorialIterator it = usuario.getHistorialIteratorFiltrado("busqueda");
            if (it.estaVacio()) {
                cout << "  No hay búsquedas en el historial.\n";
            } else {
                int idx = 1;
                while (it.haySiguiente()) {
                    HistorialEntry entry = it.siguiente();
                    cout << "  " << idx << ". \"" << entry.query << "\"  (" << entry.timestamp << ")\n";
                    idx++;
                }
            }
            esperarEnter();
        }
        else if (opcion == '3') {
            limpiarPantalla();
            cout << "═══ TODO EL HISTORIAL ═══\n\n";
            HistorialIterator it = usuario.getHistorialIterator();
            if (it.estaVacio()) {
                cout << "  El historial está vacío.\n";
            } else {
                int idx = 1;
                while (it.haySiguiente()) {
                    HistorialEntry entry = it.siguiente();
                    cout << "  " << idx << ". ";
                    if (entry.tipo == "pelicula") {
                        cout << "🎬 ";
                        auto itPeli = pelis.find(entry.movieId);
                        if (itPeli != pelis.end())
                            cout << itPeli->second.getTtitle();
                        else
                            cout << "Película ID " << entry.movieId;
                    } else {
                        cout << "🔍 \"" << entry.query << "\"";
                    }
                    cout << "  (" << entry.timestamp << ")\n";
                    idx++;
                }
            }
            esperarEnter();
        }
        else if (opcion == '4') {
            usuario.guardarSnapshot();
            limpiarPantalla();
            cout << "╔═══════════════════════════════════════════════════════════╗\n";
            cout << "║   ✔ SNAPSHOT GUARDADO CON EXITO                         ║\n";
            cout << "╚═══════════════════════════════════════════════════════════╝\n";
            esperar(2);
        }
        else if (opcion == '5') {
            limpiarPantalla();
            cout << "═══ SNAPSHOTS DISPONIBLES ═══\n\n";
            if (usuario.cantidadSnapshots() == 0) {
                cout << "  No hay snapshots guardados.\n";
                esperarEnter();
            } else {
                usuario.mostrarSnapshots();
                cout << "\n  👉 Selecciona el índice del snapshot a restaurar (o 'c' para cancelar): ";
                char resp;
                cin >> resp;
                if (resp != 'c' && resp != 'C') {
                    int idx = resp - '0';
                    if (idx >= 0 && idx < usuario.cantidadSnapshots()) {
                        usuario.restaurarSnapshot(idx);
                        cout << "\n  ✔ Snapshot restaurado con éxito.\n";
                    } else {
                        cout << "\n  ⚠ Índice inválido.\n";
                    }
                    esperar(2);
                }
            }
        }
    } while (opcion != '0');
}