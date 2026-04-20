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
    Movie(int _id,string _title,string _year,string _origin,string _director,string _cast,string _genre,string wiki,string _plot);
};

vector<Movie> leerCSV(const string& csv);







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





#endif //PROYECTAZO_CCLASES_H