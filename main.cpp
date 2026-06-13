#include "Utilidades.h"
#include "Interfaz.h"
#include "Singleton.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    system("cls");

    // Conexion con el patron Singleton:
    // motor es la unica instancia del gestor de busqueda en todo el programa.
    // Desde aqui se cargan los CSV, se limpian los datos y se construye el Trie.
    Singleton& motor = Singleton::getInstancia();
    motor.cargarDatos("peliculas.csv", "datosLimpios.csv");

    cout << "\n===== PRUEBA DEL TRIE =====\n";
    string consulta;
    while(true)
    {
        cout << "\nBuscar: ";
        getline(cin, consulta);
        if(consulta == "exit")
            break;
        vector<int> resultados = motor.buscar(consulta);
        if(resultados.empty())
        {
            cout << "Sin resultados\n";
            continue;
        }
        cout << "\nResultados:\n";
        for(int id : resultados)
        {
            const DataLimpia* pelicula = motor.obtenerPelicula(id);
            if (pelicula != nullptr) {
                cout
                    << id
                    << " -> "
                    << pelicula->getTitle()
                    << endl;
            }
        }
    }

















    /*
    char opcion_entrada;
    inicio();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name;
    InicioSesionAndRegistro(us_email,us_password,us_name,opcion_entrada);
    limpiarPantalla();
    char g;
    pa
    ntallaPrincipal(us_name,pelis,g);
    */

    return 0;
}
