#include "CClases.h"
#include "Plimpieza.h"
#include "Interfaz.h"
#include "Funciones.h"


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    system("chcp 65001 > nul");
    system("cls");
    cout << "Leyendo archivos..." << endl;
    unordered_map<int,Movie> pelis = leerPeliculas("peliculas.csv"); // Usa el archivo real de plots si lo necesitas

    cout << "Limpiando datos y preparando texto..." << endl;
    unordered_map<int , DataLimpia> dataLimpia;
    exportarDataLimpiaCSV(pelis,"datosLimpios.csv", dataLimpia);


    char opcion_entrada;
    inicio();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name;
    InicioSesionAndRegistro(us_email,us_password,us_name,opcion_entrada);
    limpiarPantalla();
    char g;
    pantallaPrincipal(us_name,pelis,g);

    return 0;
}