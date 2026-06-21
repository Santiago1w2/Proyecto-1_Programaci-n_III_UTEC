#include "Utilidades.h"

Movie::Movie() = default;
Movie::Movie(string _year,string _title, string _origin,string _director ,string _cast, string _genre, string wiki, string _plot) {
    title = _title;
    release_year = _year;
    origin = _origin;
    director = _director;
    cast = _cast;
    genre = _genre;
    wiki_page = wiki;
    plot = _plot;
}


DataLimpia::DataLimpia() = default;
DataLimpia::DataLimpia(string& _title,string& _release_year,string& _origin,string& _director,
    string& _cast,string& _genre,string& _plot, Movie* _movie) {
    title = _title;
    release_year = _release_year;
    origin = _origin;
    director = _director;
    cast = _cast;
    genre = _genre;
    plot = _plot;
    movie = _movie;
}

//Se usa cuando un usuario quiere información específica de una película
void DataLimpia::more_info() {
    cout << "Title: " << title << endl;
    cout << "Release Year: " << release_year << endl;
    cout << "Origin/Ethnicity: " << origin << endl;
    cout << "Director: " << director << endl;
    cout << "Cast: " << cast << endl;
    cout << "Genre: " << genre << endl;
    cout << "Plot: " << plot << endl;
}


Usuario::Usuario(string &user, string &_email, string &pass, unordered_map<int,Movie> &VMT,
    unordered_map<int,Movie> &MG, unordered_map<int,Movie> &Ban, unordered_map<int,Movie> &hist) {
    username=user;
    email = _email;
    password = pass;
    verMasTarde = VMT;
    meGusta = MG;
    baneado = Ban;
    historial = hist;
}


//Convierte un string a minsculas
string aMinuscula(string texto){
    for(char& c : texto) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return texto;
}
