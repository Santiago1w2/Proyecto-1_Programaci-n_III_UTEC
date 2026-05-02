#include "CClases.h"
#include "Interfaz.h"

int main() {
    //Inicializacion de peliculas y usuarios como registro histórico
    cout << "Leyendo archivos..." << endl;

    map<int,Movie> pelis = leerPeliculas("peliculas.csv"); //Pre-procesamiento de las peliculas
    vector<Usuario> usuarios = leerUsuarios("registroUsuarios.txt",pelis);

    //Menu inicial de la plataforma
    char opcion_entrada;
    inicio();
    seleccionar_opcion(opcion_entrada);
    limpiarPantalla();
    string us_email,us_password,us_name;
    //Usuario ya existente quiere
    //iniciar sesion
    if (opcion_entrada=='a') {
        inicio_sesion(us_email,us_password);
        while (!validar_info(us_email,us_password)) { //Verifica si la combinacion correo-contraseña son iguales a las registradas
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
        while (validar_correo(us_email)) { //Verifica si el correo ya esta registrado
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
    cout << "cargando data ....." << endl;
    for (const auto& [clave, valor]: dataLimpia) {
        trie.insertar(valor,clave);
    }
    cout<<"\n\n\n----------- BIENVENIDO -----------------\n\n";
    cout<<string(100,'=')<<endl;
    cout<<string(30,' ')<<"PELICULAS RECOMENDADAS"<<endl;
    cout<<string(100,'=')<<endl;
    //peliculasRecomendadas(us_email,pelis);

    //Incluir recomendaciones personalizadas basado en busquedas previas
    // Top 5

    /*
    cout << "-------------------------------------" << endl;
    cout << "Top 5"<< endl;
    cout << "-------------------------------------" << endl;
    for (int j = 0; j<6; j++) {
        cout <<"("<<j+1<<") "<<pelis[j].getTtitle() << " ";
    }
    */



    cout<< endl;
    cin.ignore(); // si no se ua esto el getline va a fallar por usar cin antes
    vector<Movie> pelis2;
    string buscar;
    int number = 0;
    while (number < 10) {
        cout << "---------\n";
        cout << "Buscar: ";
        getline(cin, buscar);
        cout << "----------\n";
        vector<int> resultados = trie.buscar(buscar);
        for (int i = 0; i < resultados.size(); i++) {
            cout << "[" << resultados[i] << "] " << pelis[resultados[i]].getTtitle() << endl;  }

        number++;
    }



    system("pause");
}
