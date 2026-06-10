#ifndef PROYECTAZO_CCLASES_H
#define PROYECTAZO_CCLASES_H

//Se importanlibrerias que se usan a nivel global en el programa
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
#include <string_view>
#include <array>
#include <thread>
#include <memory>
#include <mutex>
#include <chrono>
#include <conio.h>
#include <windows.h>
using namespace std;
inline constexpr int MAX_LONG = 6;
inline constexpr int NUM_THREADS = 8;


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
    string getYear()const {return release_year;}
    string getTtitle() const {return title;}
    string getDirector()const  {return director;}
    string getGenre() const {return genre;}
    string getOrigin() const {return origin;}
    string getPlot() const {return plot;}
    string getWiki() const {return wiki_page;}
    string getCast() const {return cast;}
};




class DataLimpia {
    Movie* movie;
    string title;
    string release_year;
    string origin;
    string director;
    string cast;
    string genre;
    string plot;
public:
    DataLimpia();
    DataLimpia(string& _title,string& _release_year,string& _origin,string& _director,
        string& _cast,string& _genre,string& _plot, Movie* _movie);
    string getTitle()const{return title;}
    string getRelease_year()const{return release_year;}
    string getOrigin()const{return origin;}
    string getDirector()const{return director;}
    string getCast()const{return cast;}
    string getGenre()const{return genre;}
    string getPlot()const{return plot;}
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
    Usuario(string &user, string &_email, string &pass, unordered_map<int,Movie> &VMT,
        unordered_map<int,Movie> &MG, unordered_map<int,Movie> &Ban, unordered_map<int,Movie> &hist);

};

struct TokenInfo {
    string token;
    int peso;
};

struct DocumentoIndexado {
    int movieID;
    vector<TokenInfo> tokens;
};

string aMinuscula(string texto);



#endif //PROYECTAZO_CCLASES_H