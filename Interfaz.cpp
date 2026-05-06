//
// Created by smora on 29/04/2026.
//

#include "Interfaz.h"

void moverCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void limpiarPantalla() {
    system("cls");
}

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
    int espacios1 = max(0, 110 - (int)nombre.size());

    // ===== INTERFAZ =====
    cout << "╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮\n";
    cout << "│ 🎬 UTECFLIX";

    // usuario dinámico (alineado a la derecha)
    cout << string(espacios1, ' ') << "Usuario: " << nombre << " │\n";

    cout << "╰───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\n\n";

    cout << "╭─────────────── MENÚ ───────────────╮   ╭──────────────────── CONTENIDO ───────────────────────────────────────────────────────────────────────╮\n";
    cout << "│                                    │   │   🎥 RECOMENDACIONES                                                                                 │\n";
    cout << "│  A. 👤 Mi Perfil                   │   │   ───────────────────────────────                                                                    │\n";
    cout << "│  B. 🕘 Historial                   │   │                                                                                                      │\n";
    cout << "│  C. ⭐ Favoritos                   │   │                                                                                                      │\n";
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