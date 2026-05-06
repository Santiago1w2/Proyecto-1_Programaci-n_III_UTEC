#include "CClases.h"
#include "Interfaz.h"
#include "PLimpieza.h"
#include "Trie.h"


int main() {
    cout << "Leyendo archivos..." << endl;
    unordered_map<int,Movie> pelis = leerPeliculas("peliculas.csv"); // Usa el archivo real de plots si lo necesitas

    // Ojo: Si 'registroUsuarios.txt' requiere std::map en leerUsuarios, revisa que no haya conflicto (ahora usamos unordered_map en CClases)
    // vector<Usuario> usuarios = leerUsuarios("registroUsuarios.txt", pelis);

    cout << "Limpiando datos y preparando texto..." << endl;
    unordered_map<int , DataLimpia> dataLimpia;
    exportarDataLimpiaCSV(pelis,"datosLimpios.csv", dataLimpia);


    // PREPARA TODOS LOS DATOS LIMPIOS SIN IMPRIMIRLOS Y CONSTRUYE LA ESTRUCTURA

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

    limpiarPantalla();
    Trie trie;

    cout << "Cargando data al Arbol Trie ....." << endl;
    for (auto&[id,valor]:dataLimpia) {
        trie.insertarCompleto(valor.title,id,5);
        trie.insertarCompleto(valor.release_year,id,1);
        trie.insertarCompleto(valor.origin,id,1);
        trie.insertarCompleto(valor.director,id,1);
        trie.insertarCompleto(valor.cast,id,1);
        trie.insertarCompleto(valor.genre,id,1);
        trie.insertarCompleto(valor.plot,id,1);
    }


    cout<<"\n\n\n----------- BIENVENIDO -----------------\n\n";
    cout<<string(100,'=')<<endl;
    cout<<string(30,' ')<<"PELICULAS RECOMENDADAS"<<endl;
    cout<<string(100,'=')<<endl;

    cout<< endl;
    cin.ignore();

    string query;
    int number = 0;
    while (number < 10) {
        cout << "---------\n";
        cout << "Buscar: ";
        getline(cin, query);
        cout << "----------\n";
        vector<int> result = trie.buscar(query);
        for (auto& i : result) {
            cout << "[" << i << "]" << pelis[i].getTtitle()<< endl;
        }
    }

    system("pause");
    return 0;
}