#include "Utilidades.h"
#include "LimPelis.h"
#include "Interfaz.h"
#include "Procesador.h"
#include "Proxy.h"

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
    char opcion_entrada;
    inicio();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name,us_edad;
    InicioSesionAndRegistro(us_email,us_password,us_name,us_edad,opcion_entrada,dataSucia);
    if (us_email.empty()) {
        cout << "No se inicio sesion.\n";
        return 0;
    }
    limpiarPantalla();
    int edad = calcularEdad(us_edad);
    if (edad < 0) edad = 18;
    char g;
    pantallaPrincipal(us_name,dataSucia,edad,g);
    cin.ignore();
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
        registrarBusquedaUsuario(us_email, consulta);
        vector<int> resultados =preprocesador.buscar(consulta);
        if(resultados.empty())
        {
            cout << "Sin resultados\n";
            continue;
        }
        cout << "\nResultados:\n";
        bool hayResultadosVisibles = false;
        for(int id : resultados)
        {
            const Movie& pelicula = dataSucia[id];
            PeliculaReal real(&pelicula);
            PeliculaProxy proxy(&real, edad);
            IPelicula& peliculaVisible = proxy;
            if (!peliculaVisible.puedeMostrarse()) {
                continue;
            }

            cout
                << id
                << " -> "
                << peliculaVisible.getTitulo()
                << endl;
            hayResultadosVisibles = true;
        }

        if (!hayResultadosVisibles) {
            cout << "Sin resultados disponibles para tu edad\n";
        }
    }
    return 0;
}
