#include "Utilidades.h"
#include "LimPelis.h"
#include "Interfaz.h"
#include "Procesador.h"
#include <thread>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    system("cls");
    cout << "Leyendo archivos..." << endl;
    unordered_map<int,Movie> dataSucia;
    unordered_map<int , DataLimpia> dataLimpia;
    Procesador preprocesador;
    bool datosListos;
    thread t(cargarData,ref(preprocesador),ref(dataSucia),ref(dataLimpia),ref(datosListos));


    char opcion_entrada;
    Home();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name;
    InicioSesionAndRegistro(us_email,us_password,us_name,opcion_entrada);
    limpiarPantalla();
    cout << "Cargando Data";
    while (!datosListos) {
        cout << "." << flush;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    t.join();
    limpiarPantalla();

    char opcion_menu;
    pantallaPrincipal(us_name,dataSucia,opcion_menu);


    switch (opcion_menu) {
        case 'D':
            interfaz_buscar(dataSucia,preprocesador);
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
    }


















    /*
    cout << "\n===== PRUEBA DEL TRIE =====\n";

    */

    return 0;
}