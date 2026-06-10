#include "Utilidades.h"
#include "LimPelis.h"
#include "Interfaz.h"
#include "Procesador.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    system("cls");
    cout << "Leyendo archivos..." << endl;
    unordered_map<int,Movie> dataSucia = leerPeliculas("peliculas.csv");

    cout << "Limpiando datos y preparando texto..." << endl;
    unordered_map<int , DataLimpia> dataLimpia;
    exportarDataLimpiaCSV(dataSucia,"datosLimpios.csv", dataLimpia);

    Procesador preprocesador;
    cout << "Procesando datos para subir al Trie..."<< endl;

    auto inicio = std::chrono::high_resolution_clock::now();
    preprocesador.procesar(dataLimpia);
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(fin - inicio);
    cout<<duracion;

    cout << "\n===== PRUEBA DEL TRIE =====\n";
    string consulta;
    while(true)
    {
        cout << "\nBuscar: ";
        getline(cin, consulta);
        if(consulta == "exit")
            break;
        vector<int> resultados =preprocesador.buscar(consulta);
        if(resultados.empty())
        {
            cout << "Sin resultados\n";
            continue;
        }
        cout << "\nResultados:\n";
        for(int id : resultados)
        {
            cout
                << id
                << " -> "
                << dataLimpia[id].getTitle()
                << endl;
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