//
// Created by burgo on 27/3/2026.
//

#ifndef PROYECTAZO_CCLASES_H
#define PROYECTAZO_CCLASES_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;


class Movie {
    int id;
    string title;
    string release_year;
    string origin;
    string director;
    string cast;
    string genre;
    string wiki_page;
    string plot;
public:
    Movie(int _id,string _year,string _title,string _origin,string _director,string _cast,string _genre,string wiki,string _plot);
    int getId() const;
    string getYear();
    string getTtitle();
    string getOrigin();
    string getDirector();
    string getGenre();
    string getWiki();
    string getPlot();

    void more_info();

};



vector<Movie> leerCSV(const string& csv);

void procesarComillas(stringstream& ss,string& name);
class Usuario {
    string username;
    string email;
    string password;
    vector<int> verMasTarde;
    vector<int> MeGusta;
    vector<int> Baneado;

public:
    Usuario(string user,string _email,string _password):username(user),email(_email),password(_password){}
    void revisarUsuario();

};


bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);
void registrar_usuario(const string& name, const string& email, const string& clave);
bool validar_usuario(const string& _username);
int busquedaBinaria(const vector<Movie>& v, int objetivo_id);
vector<string> mostrar_usuarios();


//Strcutra del arbol (pruebas)

struct Nodo{
    unordered_map<char, Nodo*> nodos; // unordered_map, es como un mpa, pero no lo ordena, como dijo el profe map ordena, este no
    vector<Movie> peliculas;
    //forma de los donos
};

class Arbol {
    Nodo* raiz;
public:
    Arbol();
    void insertar(Movie nuevo);
    vector<Movie> buscar1(string query);
};




#endif //PROYECTAZO_CCLASES_H