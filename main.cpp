#include "CClases.h"
#include "Pprocesamiento.h"
#include "Interfaz.h"


int main() {
    cout << "Leyendo archivos..." << endl;
    unordered_map<int,Movie> pelis = leerPeliculas("peliculas.csv"); // Usa el archivo real de plots si lo necesitas

    // Ojo: Si 'registroUsuarios.txt' requiere std::map en leerUsuarios, revisa que no haya conflicto (ahora usamos unordered_map en CClases)
    // vector<Usuario> usuarios = leerUsuarios("registroUsuarios.txt", pelis);

    cout << "Limpiando datos y preparando texto..." << endl;

    // PREPARA TODOS LOS DATOS LIMPIOS SIN IMPRIMIRLOS Y CONSTRUYE LA ESTRUCTURA
    unordered_map<int, string> dataLimpia = prepararDataLimpia(pelis);

    char opcion_entrada;
    inicio();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name;
    InicioSesionAndRegistro(us_email,us_password,us_name,opcion_entrada);

    return 0;
}