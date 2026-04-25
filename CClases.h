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
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <random>
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
    string getYear()const;
    string getTtitle()const;
    string getOrigin() const;
    string getDirector() const;
    string getGenre() const;
    string getWiki() const;
    string getPlot()const;

    void more_info();
};

void procesarComillas(stringstream& ss,string& name);
class Usuario {
    string username;
    string email;
    string password;
    vector<Movie> verMasTarde;
    vector<Movie> meGusta;
    vector<Movie> baneado;
    vector<Movie> historial;
public:
    Usuario(const string &user, const string &_email, const string &_pass, const vector<Movie> &VT, const vector<Movie> &MG, const vector<Movie> &Ban, const vector<Movie> &hist);
    void revisarUsuario();
};

vector<Movie> leerPeliculas(const string& csv);
vector<Movie> generarPelis(string linea,const vector<Movie>& pelis);
vector<Usuario> leerUsuarios(const string& csv,vector<Movie> pelis);



bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);
bool validar_usuario(const string& _username); //No es necesario

void registrar_nuevoUsuario(const string& name, const string& email, const string& clave);
void actualizarUsuario(vector<int> pelis,string tipo);

void peliculasRecomendadas(const string &_email,const vector<Movie>& pelis);

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