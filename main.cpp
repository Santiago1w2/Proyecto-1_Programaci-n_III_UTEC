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
        esperar(2);
        cout << R"(
╭───────────────────────────────────╮
│  ✔ USUARIO REGISTRADO CON EXITO   │
╰───────────────────────────────────╯
)";
        limpiarPantalla();
        registrar_nuevoUsuario(us_name,us_email,us_password);
    }

    Trie trie;
    limpiarPantalla();
    cout << "Cargando data al Arbol Trie ....." << endl;
    for (const auto& [clave, valor]: dataLimpia) {
        // En el trie: insertar(texto_info, ID)
        trie.insertar(valor, clave);
    }
    construirIndice(dataLimpia); // Crear el índice de palabras intermedios

    cout<<"\n\n\n----------- BIENVENIDO -----------------\n\n";
    cout<<string(100,'=')<<endl;
    cout<<string(30,' ')<<"PELICULAS RECOMENDADAS"<<endl;
    cout<<string(100,'=')<<endl;

    cout<< endl;
    cin.ignore();

    string buscar;
    int number = 0;
    while (number < 10) {
        cout << "---------\n";
        cout << "Buscar: ";
        getline(cin, buscar);
        cout << "----------\n";
        vector<int> resultados = trie.buscar(buscar);

        if (resultados.empty()){
            cout << "No se encontraron coincidencias.\n";
        } else {
            for (int i = 0; i < resultados.size(); i++) {
                cout << "[" << resultados[i] << "] " << pelis[resultados[i]].getTtitle() << endl;
            }
        }
        number++;
    }

    system("pause");
    return 0;
}