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
    unordered_map<int,Movie> verMasTarde;
    unordered_map<int,Movie> meGusta;
    unordered_map<int,Movie> baneado;
    unordered_map<int,Movie> historial;
public:
    Usuario(const string &user, const string &_email, const string &_pass, const unordered_map<int,Movie> &VT, const unordered_map<int,Movie> &MG, const unordered_map<int,Movie> &Ban, const unordered_map<int,Movie> &hist);
    void revisarUsuario();
};
string aMinusculas(string texto);
unordered_map<int,Movie> leerPeliculas(const string& csv);
unordered_map<int, Movie> convertirAPelis(const vector<int>& ids, const unordered_map<int, Movie>& pelis);
vector<Usuario> leerUsuarios(const string &csv, const unordered_map<int,Movie>& pelis);

vector<int> parseLista(const string& s);

bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);
bool validar_usuario(const string& _username); //No es necesario

void registrar_nuevoUsuario(const string& name, const string& email, const string& clave);
void actualizarUsuario(vector<int> pelis,string tipo);

void peliculasRecomendadas(const string &_email,const vector<Movie>& pelis);

int busquedaBinaria(const vector<Movie>& v, int objetivo_id);
vector<string> mostrar_usuarios();


vector<string> separar(const string& texto);

// Nodo del Suffix Trie
// - hijos:          mapa char -> nodo siguiente
// - movieIds:       IDs de peliculas cuyo sufijo TERMINA en este nodo
// - esFinDePalabra: true si algun sufijo termina aqui
struct Nodo {
    unordered_map<char, Nodo*> hijos;
    vector<int> movieIds;
    bool esFinDePalabra;

    // Constructor: inicializa esFinDePalabra en false
    // Sin esto el valor es basura de memoria -> comportamiento indefinido
    Nodo() : esFinDePalabra(false) {}
};

// Suffix Trie:
//   insertar(info, id) -> separa en palabras, por cada palabra
//                         inserta TODOS sus sufijos con el id al final
//   buscar(query)      -> navega hasta el nodo del query, hace DFS
//                         recolectando todos los ids descendientes
class Trie {
    Nodo* raiz;
public:
    Trie();
    void insertar(const string& info, int id);
    vector<int> buscar(const string& query);
};

// Vacia intencionalmente: el Suffix Trie reemplaza el indice separado
void construirIndice(const unordered_map<int, string>& dataLimpia);


#endif //PROYECTAZO_CCLASES_H