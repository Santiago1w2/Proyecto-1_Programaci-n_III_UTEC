#include "Interfaz.h"

void moverCursor(int x, int y) {
    COORD coord;
    coord.X = static_cast<SHORT>(x);
    coord.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void limpiarPantalla() {
    system("cls");
}

void esperarEnter() {
    cout << "\nPresiona Enter para continuar...";
    cin.get();
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

static void imprimirOpcionMenu(const string& texto, bool seleccionado) {
    cout << "| ";
    if (seleccionado) {
        setColor(240);
        cout << ">> " << left << setw(31) << texto;
        setColor(7);
    } else {
        cout << "   " << left << setw(31) << texto;
    }
    cout << " |\n";
}

static void moverSeleccion(int& opcion, int total, int tecla) {
    if (tecla == 72 || tecla == 'w' || tecla == 'W') {
        opcion--;
    } else if (tecla == 80 || tecla == 's' || tecla == 'S') {
        opcion++;
    }

    if (opcion < 1) {
        opcion = total;
    } else if (opcion > total) {
        opcion = 1;
    }
}

static int leerTeclaMenu() {
    int tecla = getch();
    if (tecla == 0 || tecla == 224) {
        return getch();
    }
    return tecla;
}

void inicio() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
    limpiarPantalla();

    setColor(11);
    cout << "\n";
    cout << "============================================================\n";
    cout << "                         UTECFLIX                           \n";
    cout << "============================================================\n";
    setColor(14);
    cout << "                 Peliculas, busqueda y recomendaciones       \n";
    setColor(7);
    esperarEnter();
    limpiarPantalla();
}

void mostrarMenu(int opcion) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
    limpiarPantalla();

    cout << "+--------------------------------------+\n";
    cout << "|          INGRESO AL SISTEMA          |\n";
    cout << "+--------------------------------------+\n";
    imprimirOpcionMenu("Iniciar sesion", opcion == 1);
    imprimirOpcionMenu("Registrarse", opcion == 2);
    cout << "+--------------------------------------+\n";
    cout << "Controles: flechas o W/S para moverte, ENTER para elegir.\n";
    cout << "Atajos: 1 iniciar sesion, 2 registrarse.\n";
}

void seleccionar_opcion(char& op) {
    int opcion = 1;

    while (true) {
        mostrarMenu(opcion);
        int tecla = leerTeclaMenu();

        if (tecla == '1') {
            opcion = 1;
            op = 'a';
            limpiarPantalla();
            break;
        }
        if (tecla == '2') {
            opcion = 2;
            op = 'b';
            limpiarPantalla();
            break;
        }
        if (tecla == 13) {
            op = opcion == 1 ? 'a' : 'b';
            limpiarPantalla();
            break;
        }

        moverSeleccion(opcion, 2, tecla);
    }
}

void mostrar_inicio_sesion() {
    limpiarPantalla();
    cout << "+--------------------------------------+\n";
    cout << "|             INICIO SESION            |\n";
    cout << "+--------------------------------------+\n";
}

void inicio_sesion(string& correo, string& pass) {
    mostrar_inicio_sesion();
    cout << "Email: ";
    cin >> correo;
    cout << "Password: ";
    cin >> pass;
}

void esperar(int n) {
    this_thread::sleep_for(chrono::seconds(n));
}

void mostrar_registro_usuario() {
    limpiarPantalla();
    cout << "+--------------------------------------+\n";
    cout << "|          REGISTRO DE USUARIO         |\n";
    cout << "+--------------------------------------+\n";
}

void registro(string& correo, string& pass, string& name, string& clave) {
    mostrar_registro_usuario();
    cout << "User name: ";
    cin >> name;
    cout << "Email: ";
    cin >> correo;
    cout << "Password: ";
    cin >> pass;
    cout << "Repetir password: ";
    cin >> clave;
}

void InicioSesionAndRegistro(string& us_email, string& us_password, string& us_name, char& opcion_entrada) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");

    if (opcion_entrada == 'a') {
        inicio_sesion(us_email, us_password);
        while (!validar_info(us_email, us_password)) {
            limpiarPantalla();
            setColor(12);
            cout << "Credenciales incorrectas. Intenta nuevamente.\n";
            setColor(7);
            esperar(1);
            inicio_sesion(us_email, us_password);
        }
        us_name = UserName(us_email, us_password);
        limpiarPantalla();
        setColor(10);
        cout << "Inicio de sesion exitoso. Bienvenido, " << us_name << ".\n";
        setColor(7);
        esperar(1);
        limpiarPantalla();
        return;
    }

    string clave_temp;
    registro(us_email, us_password, us_name, clave_temp);

    while (validar_correo(us_email)) {
        limpiarPantalla();
        setColor(12);
        cout << "Ese correo ya esta registrado.\n";
        setColor(7);
        esperar(1);
        registro(us_email, us_password, us_name, clave_temp);
    }

    while (us_password != clave_temp) {
        limpiarPantalla();
        setColor(12);
        cout << "Las claves no coinciden. Vuelve a intentarlo.\n";
        setColor(7);
        esperar(1);
        registro(us_email, us_password, us_name, clave_temp);
    }

    registrar_nuevoUsuario(us_name, us_email, us_password);
    limpiarPantalla();
    setColor(10);
    cout << "Usuario registrado con exito. Bienvenido, " << us_name << ".\n";
    setColor(7);
    esperar(1);
    limpiarPantalla();
}

void pantallaPrincipal(const string& nombre, const unordered_map<int, Movie>& pelis, char& n) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");

    int opcion = 1;
    const array<char, 5> opciones = {'A', 'B', 'C', 'D', '0'};

    while (true) {
        limpiarPantalla();

        cout << "============================================================\n";
        cout << "                          UTECFLIX                          \n";
        cout << "============================================================\n";
        cout << "Usuario: " << nombre << "\n";
        cout << "Peliculas cargadas: " << pelis.size() << "\n\n";

        cout << "+---------------- MENU ----------------+\n";
        imprimirOpcionMenu("Mi perfil", opcion == 1);
        imprimirOpcionMenu("Historial", opcion == 2);
        imprimirOpcionMenu("Recomendaciones", opcion == 3);
        imprimirOpcionMenu("Buscar pelicula", opcion == 4);
        imprimirOpcionMenu("Salir", opcion == 5);
        cout << "+--------------------------------------+\n";
        cout << "Controles: flechas o W/S para moverte, ENTER para elegir.\n";
        cout << "Atajos: A perfil, B historial, C recomendaciones, D buscar, 0 salir.\n\n";

        cout << "Recomendaciones rapidas:\n";
        peliculasRecomendadasPanel(pelis);

        int tecla = leerTeclaMenu();
        if (tecla == 13) {
            n = opciones[opcion - 1];
            return;
        }

        if (tecla == 'a' || tecla == 'A') {
            n = 'A';
            return;
        }
        if (tecla == 'b' || tecla == 'B') {
            n = 'B';
            return;
        }
        if (tecla == 'c' || tecla == 'C') {
            n = 'C';
            return;
        }
        if (tecla == 'd' || tecla == 'D') {
            n = 'D';
            return;
        }
        if (tecla == '0') {
            n = '0';
            return;
        }

        moverSeleccion(opcion, static_cast<int>(opciones.size()), tecla);
    }
}
