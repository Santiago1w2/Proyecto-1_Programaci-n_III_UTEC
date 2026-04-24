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
    while (tolower(opcion_entrada)!='a' and tolower(opcion_entrada)!='a') {
        cout<<"\n!ERRO EN DATO INGRESADO!";
        cout<<"\n'a' -> Iniciar sesion";
        cout<<"\n'b' -> Registrarse";
        cout<<"\n|-> ";cin>>opcion_entrada;
    }
    string us_email,us_password,us_name;
    if (tolower(opcion_entrada)=='a') {
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
    }

    cout<<"----------- BIENVENIDO -----------------";
    return 0;
}
