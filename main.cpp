#include "Utilidades.h"
#include "LimPelis.h"
#include "Interfaz.h"
#include "Procesador.h"
#include "Singleton.h"
#include <thread>
#include <exception>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    system("cls");
    cout << "Leyendo archivos..." << endl;
    CatalogoPeliculas& catalogo = CatalogoPeliculas::instancia();
    atomic_bool datosListos = false;
    exception_ptr errorCarga = nullptr;
    thread t([&catalogo, &datosListos, &errorCarga]() {
        try {
            auto inicio = chrono::high_resolution_clock::now();
            catalogo.cargarDatos("peliculas.csv", "datosLimpios.csv");
            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
            cout << duracion << endl;
        } catch (...) {
            errorCarga = current_exception();
        }
        datosListos = true;
    });


    char opcion_entrada;
    Home();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name;
    string fechaNacUsuario = "01/01/2000";
    InicioSesionAndRegistro(us_email,us_password,us_name,fechaNacUsuario,opcion_entrada);
    int edadUsuario = calcularEdad(fechaNacUsuario);
    if (edadUsuario < 0) edadUsuario = 18;
    limpiarPantalla();
    const string base = "Cargando data";
    const vector<string> anim = {
        "   ",
        ".  ",
        ".. ",
        "...",
        "...."
    };

    while (!datosListos) {
        for (const auto& a : anim) {
            cout << "\r" << base << " " << a << flush;
            this_thread::sleep_for(chrono::milliseconds(300));
        }
    }
    t.join();
    if (errorCarga) {
        try {
            rethrow_exception(errorCarga);
        } catch (const exception& e) {
            cout << "\nError cargando peliculas: " << e.what() << endl;
            return 1;
        }
    }

    bool runnig = true;
    while (runnig) {
        char opcion_menu;
        pantallaPrincipal(us_name,catalogo.peliculas(),opcion_menu,edadUsuario);
        switch (opcion_menu) {
            case 'A': {
                mostrarPerfilUsuario(us_email,us_password,us_name,fechaNacUsuario,edadUsuario);
                break;
            }
            case 'D': {
                interfaz_buscar(catalogo.peliculas(),catalogo.motorBusqueda(),us_email,edadUsuario);
                break;
            }
            case 'B': {
                mostrarHistorialUsuario(us_email);
                break;
            }
            case 'C': {
                mostrarFavoritosUsuario(us_email,catalogo.peliculas());
                break;
            }
            case '0': {
                runnig = false;
                break;
            }
            default:
                cout << "Opcion no valida" << endl;
                limpiarPantalla();
                break;
        }
    }



















    /*
    cout << "\n===== PRUEBA DEL TRIE =====\n";

    */

    return 0;
}
