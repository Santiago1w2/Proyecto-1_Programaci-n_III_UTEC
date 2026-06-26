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
|Fecha de Nac. (DD/MM/AAAA):                         |
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

void registro(string& correo, string& pass, string& name, string& clave, string& edad) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");

    // Paso 1: Nombre de usuario
    system("cls");
    cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|  Paso 1/5: Ingresa tu nombre de usuario            |
├────────────────────────────────────────────────────┤
|                                                     |
╰────────────────────────────────────────────────────╯
)";
    moverCursor(1, 6);
    cin >> ws;
    getline(cin, name);

    // Paso 2: Correo electrónico
    system("cls");
    cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|  Paso 2/5: Ingresa tu correo electrónico            |
├────────────────────────────────────────────────────┤
|                                                     |
╰────────────────────────────────────────────────────╯
)";
    moverCursor(1, 6);
    cin >> correo;

    // Paso 3: Fecha de nacimiento
    do {
        system("cls");
        cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|  Paso 3/5: Ingresa tu fecha de nacimiento           |
|            (DD/MM/AAAA) ej: 15/03/2000                |
├────────────────────────────────────────────────────┤
|                                                     |
╰────────────────────────────────────────────────────╯
)";
        moverCursor(1, 7);
        cin >> edad;
        if (calcularEdad(edad) == -1) {
            system("cls");
            cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|  ⚠ FECHA INVALIDA                                  |
|  La fecha no existe o tiene formato incorrecto.     |
├────────────────────────────────────────────────────┤
|                                                     |
╰────────────────────────────────────────────────────╯
)";
            esperar(2);
        }
    } while (calcularEdad(edad) == -1);

    // Paso 4: Contraseña
    system("cls");
    cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|  Paso 4/5: Ingresa tu contraseña                    |
├────────────────────────────────────────────────────┤
|                                                     |
╰────────────────────────────────────────────────────╯
)";
    moverCursor(1, 6);
    cin >> pass;

    // Paso 5: Repetir contraseña
    system("cls");
    cout << R"(
╭────────────────────────────────────────────────────╮
|            *REGISTRO DE NUEVO USUARIO*             |
├────────────────────────────────────────────────────┤
|  Paso 5/5: Repite tu contraseña                     |
├────────────────────────────────────────────────────┤
|                                                     |
╰────────────────────────────────────────────────────╯
)";
    moverCursor(1, 6);
    cin >> clave;
}

void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, string& us_edad, char& opcion_entrada) {
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
        registro(us_email,us_password,us_name,clave_temp,us_edad);
        while (validar_correo(us_email)) {
            limpiarPantalla();
            cout << R"(
╭───────────────────────────────╮
│    ⚠ CORREO YA REGISTRADO     │
╰───────────────────────────────╯
)"; esperar(2);
            limpiarPantalla();
            registro(us_email,us_password,us_name, clave_temp,us_edad);
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
            registro(us_email,us_password,us_name, clave_temp,us_edad);
        }
        limpiarPantalla();
        cout << R"(
╭───────────────────────────────────╮
│  ✔ USUARIO REGISTRADO CON EXITO   │
╰───────────────────────────────────╯
)";
        esperar(3);

        limpiarPantalla();
        registrar_nuevoUsuario(us_name, us_edad, us_email, us_password);
    }
}

void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, int edad, char& n) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
    system("cls");
    // ===== INTERFAZ =====
    cout << "╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║ 🎬 UTECFLIX                                                                                                                                   ║ \n";
    cout << "╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n";

    moverCursor(110,1);
    cout << "Usuario: " << nombre << " | Edad: " << edad << "\n\n";


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
    peliculasRecomendadasPanel(pelis, edad);

    // ===== INPUT =====
    moverCursor(0, 20);
    cout << "👉 Selecciona una opción: ";
    cin >> n;
}