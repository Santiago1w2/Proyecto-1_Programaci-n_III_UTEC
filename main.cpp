#include "CClases.h"

int main() {
    cout<<"-----------------------------------";
    cout<<"\n|     BIENVENIDO A UTECFLIX       |";
    cout<<"\n-----------------------------------";
    char opcion_entrada;

    cout<<"\n--- OPCIONES DE INGRESO ---";
    cout<<"\n'a' | Iniciar sesion";
    cout<<"\n'b' | Registrarse";
    cout<<"\n|-> ";cin>>opcion_entrada;
    while (tolower(opcion_entrada)!='a' and tolower(opcion_entrada)!='b') { // No habia opccion de b
        cout<<"\n!ERROR EN DATO INGRESADO!";
        cout<<"\n'a' -> Iniciar sesion";
        cout<<"\n'b' -> Registrarse";
        cout<<"\n|-> ";cin>>opcion_entrada;
    }


    string us_email,us_password,us_name;
    if (tolower(opcion_entrada)=='a') {
        // Falta implementar la opcion de seleccionar usuarios y solo poner la contraseña
        cout<<"Ingrese su correo electronico";
        cin>>us_email;
        cout<<"Ingrese su contrasenia";
        cin>>us_password;
        while (!validar_info(us_email,us_password)) { //Verifica si la combinacion correo-contraseña son iguales a las registradas
            cout<<"\n!ERROR EN LAS CREDENCIALES!\n";
            cout<<"Ingrese su correo electronico";
            cin>>us_email;
            cout<<"Ingrese su contrasenia";
            cin>>us_password;
        }
    }

    else {
        cout<<"\n-- REGISTRO DE NUEVO USUARIO --\n";
        cout << "\nIngrese si nombre de usuario: ";
        cin>>us_name;
        while (validar_usuario(us_name)) { //Verifica si el correo ya esta registrado
            cout<<"\nEL CORREO YA ESTA REGISTRADO\n";
            cout<<"Ingrese su correo electronico";
            cin>>us_name;
        }
        cout<<"\nIngrese su correo electronico";
        cin>>us_email;
        while (validar_correo(us_email)) { //Verifica si el correo ya esta registrado
            cout<<"\nEL CORREO YA ESTA REGISTRADO\n";
            cout<<"Ingrese su correo electronico";
            cin>>us_email;
        }



        string clave_temp;
        cout<<"\nIngrese su clave (contrasenia)";
        cin>>us_password;
        cout<<"Repita su clave";
        cin>>clave_temp;
        while (us_password!=clave_temp) {
            cout<<"\nLAS CLAVES NO COINCIDEN. VUELVA A INTENTAR";
            cout<<"Ingrese su clave (contrasenia)";
            cin>>us_password;
            cout<<"Repita su clave";
            cin>>clave_temp;
        }
        Usuario user1(us_name,us_email,us_password);
        registrar_usuario(us_name,us_email,us_password);
    }


    vector<Movie> pelis = leerCSV("peliculas.csv"); //Pre-procesamiento de las peliculas



    Arbol arbol;
    for (int i = 0; i < pelis.size(); i++) {
        arbol.insertar(pelis[i]);
    }
    cout<<"\n----------- BIENVENIDO -----------------\n\n";
    cout<<string(100,'=')<<endl;
    cout<<string(30,' ')<<"PELICULAS RECOMENDADAS"<<endl;
    cout<<string(100,'=')<<endl;
    peliculasRecomendadas(us_email,pelis);

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
    while (true) {
        cout << "---------\n";
        cout << "Buscar: ";
        getline(cin, buscar);
        cout << "----------\n";
        pelis2 = arbol.buscar1(buscar);

        if (!pelis2.empty()) {
            break;
        } else {
            cout << "No se encontro ninguna pelicula: " << buscar << endl;
            cout << "Intenta otra vez...\n\n";
        }
    }


    if (!pelis2.empty()) {
        for (int i=0;i<pelis2.size();i++) {
            cout << "["<< pelis2[i].getId()<< "] "<< pelis2[i].getTtitle() << endl;
        }
        int id_peli;
        cout << "Selecionar peli (id): " << endl;
        cin >> id_peli;
        int pos_id = busquedaBinaria(pelis2,id_peli);
        if (pos_id != -1) {
            pelis2[pos_id].more_info();
        }
        else {
            cout << "No se encontro la peli que se quiso seleccionar";
        }
    }
    else {
        cout << "No se encontro la pelicula: "<<buscar << endl;
    }


    return 0;
}
