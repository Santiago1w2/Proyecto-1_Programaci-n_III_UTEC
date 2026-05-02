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
#include <unordered_set>
#include <map>
#include <cctype>

using namespace std;


class Movie {
    string title;
    string release_year;
    string origin;
    string director;
    string cast;
    string genre;
    string wiki_page;
    string plot;
public:
    Movie();
    Movie(string _year,string _title,string _origin,string _director,string _cast,string _genre,string wiki,string _plot);
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
    map<int,Movie> verMasTarde;
    map<int,Movie> meGusta;
    map<int,Movie> baneado;
    map<int,Movie> historial;
public:
    Usuario(const string &user, const string &_email, const string &_pass, const map<int,Movie> &VT, const map<int,Movie> &MG, const map<int,Movie> &Ban, const map<int,Movie> &hist);
    void revisarUsuario();
};
string aMinusculas(string texto);
map<int,Movie> leerPeliculas(const string& csv);
map<int, Movie> convertirAPelis(const vector<int>& ids, const map<int, Movie>& pelis);
vector<Usuario> leerUsuarios(const string &csv, const map<int,Movie>& pelis);

vector<int> parseLista(const string& s);

bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);
bool validar_usuario(const string& _username); //No es necesario

void registrar_nuevoUsuario(const string& name, const string& email, const string& clave);
void actualizarUsuario(vector<int> pelis,string tipo);

void peliculasRecomendadas(const string &_email,const vector<Movie>& pelis);

int busquedaBinaria(const vector<Movie>& v, int objetivo_id);
vector<string> mostrar_usuarios();


//Strcutra del arbol (pruebas)
vector<string> separar(const string& texto);
struct Nodo{
    unordered_map<char, Nodo*> hijos; // unordered_map, es como un map, pero no lo ordena, como dijo el profe map ordena, este no
    unordered_set<int> movieIds;
    bool esFinDePalabra; //control duplicados
    //forma de los donos
};

class Trie {
    Nodo* raiz;
public:
    Trie();
    void insertar(const string& info, int id);
    vector<int> buscar(const string& query);
};


void construirIndice(const unordered_map<int, string>& dataLimpia);


#endif //PROYECTAZO_CCLASES_H