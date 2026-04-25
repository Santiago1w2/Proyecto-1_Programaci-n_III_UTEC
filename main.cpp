#include "CClases.h"

int main() {
    //Inicializacion de peliculas y usuarios como registro histórico
    vector<Movie> pelis = leerPeliculas("peliculas.csv"); //Pre-procesamiento de las peliculas
    vector<Usuario> usuarios = leerUsuarios("registroUsuarios.txt",pelis);

    //Menu inicial de la plataforma

    cout<<string(101,'=')<<endl;
    cout<<"||"<<string(38,' ')<<"BIENVENIDO A UTECFLIX"<<string(38,' ')<<"||\n";
    cout<<string(101,'=')<<endl;
    char opcion_entrada;
    cout<<"\n"<<string(40,'-')<<endl;
    cout<<"|->"<<string(7,' ')<<"OPCIONES DE INGRESO"<<string(8,' ')<<"<-|";
    cout<<"\n"<<string(40,'-');
    cout<<"\n'a' | Iniciar sesion"<<string(19,' ')<<"|";
    cout<<"\n'b' | Registrarse"<<string(22,' ')<<"|";
    cout<<"\n"<<string(40,'-');
    cout<<"\nOpcion :";cin>>opcion_entrada;
    while (tolower(opcion_entrada)!='a' and tolower(opcion_entrada)!='b') { // No habia opccion de b
        cout<<"\n"<<string(40,'-')<<endl;
        cout<<"!!!"<<string(4,' ')<<"ERROR EN OPCION INGRESADA"<<string(5,' ')<<"!!!";
        cout<<"\n"<<string(40,'-');
        cout<<"\n'a' | Iniciar sesion"<<string(19,' ')<<"|";
        cout<<"\n'b' | Registrarse"<<string(22,' ')<<"|";
        cout<<"\n"<<string(40,'-');
        cout<<"\nOpcion :";cin>>opcion_entrada;
    }

    string us_email,us_password,us_name;
    //Usuario ya existente quiere iniciar sesion
    if (tolower(opcion_entrada)=='a') {
        cout<<"\n"<<string(40,'-')<<endl;
        cout<<"|->"<<string(9,' ')<<"INICIO DE SESION"<<string(9,' ')<<"<-|";
        cout<<"\n"<<string(40,'-')<<endl;
        cout<<"Email :";cin>>us_email;
        cout<<"Clave :";cin>>us_password;
        cout<<string(40,'-')<<endl;
        while (!validar_info(us_email,us_password)) { //Verifica si la combinacion correo-contraseña son iguales a las registradas
            cout<<"\n"<<string(40,'-')<<endl;
            cout<<"!!!"<<string(4,' ')<<"ERROR EN LAS CREDENCIALES"<<string(4,' ')<<"!!!";
            cout<<"\n"<<string(40,'-')<<endl;
            cout<<"Email :";cin>>us_email;
            cout<<"Clave";cin>>us_password;
            cout<<string(40,'-')<<endl;
        }
        cout<<"\n"<<string(20,'=')<<string(20,'-')<<endl;
        cout<<string(8,' ')<<"INICIO DE SESION EXITOSO"<<endl;
        cout<<string(20,'-')<<string(20,'=')<<endl;
    }

    //Falta mejorar esteticamente
    //Registro de un usuario nuevo
    else {
        cout<<"\n-- REGISTRO DE NUEVO USUARIO --\n";
        cout<<"\nCorreo electronico :";cin>>us_email;
        while (validar_correo(us_email)) { //Verifica si el correo ya esta registrado
            cout<<"\nEL CORREO YA ESTA REGISTRADO\n";
            cout<<"Correo electronico :";cin>>us_email;
        }
        string clave_temp;
        cout<<"\nContrasenia :";cin>>us_password;
        cout<<"Repita su contrasenia :";cin>>clave_temp;
        while (us_password!=clave_temp) {
            cout<<"\nLAS CLAVES NO COINCIDEN. VUELVA A INTENTAR\n";
            cout<<"Contrasenia :";cin>>us_password;
            cout<<"Contrasenia :";cin>>clave_temp;
        }
        registrar_nuevoUsuario(us_name,us_email,us_password);
    }




    Arbol arbol;
    for (int i = 0; i < pelis.size(); i++) {
        arbol.insertar(pelis[i]);
    }
    cout<<"\n\n\n----------- BIENVENIDO -----------------\n\n";
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
