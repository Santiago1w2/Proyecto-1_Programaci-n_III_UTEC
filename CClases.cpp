//
// Created by burgo on 27/3/2026.
//

#include "CClases.h"
#include "Plimpieza.h"
#include <string_view>

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

Usuario::Usuario(const string &user, const string &_email, const string &_pass, const unordered_map<int,Movie> &VT, const unordered_map<int,Movie> &MG, const unordered_map<int,Movie> &Ban, const unordered_map<int,Movie> &hist) {
    username=user;
    email = _email;
    password = _pass;
    verMasTarde = VT;
    meGusta = MG;
    baneado = Ban;
    historial = hist;
}

Usuario::Usuario(){
}

string Movie::getYear()const {return release_year;}
string Movie::getTtitle() const {return title;}
string Movie::getDirector()const  {return director;}
string Movie::getGenre() const {return genre;}
string Movie::getOrigin() const {return origin;}
string Movie::getPlot() const {return plot;}
string Movie::getWiki() const {return wiki_page;}
string Movie::getCast() const {return cast;}
void Movie::more_info() {
    cout << "Title: " << title << endl;
    cout << "Release Year: " << release_year << endl;
    cout << "Origin/Ethnicity: " << origin << endl;
    cout << "Director: " << director << endl;
    cout << "Cast: " << cast << endl;
    cout << "Genre: " << genre << endl;
    cout << "Wiki Page: " << wiki_page << endl;
    cout << "Plot: " << plot << endl;
}

vector<string> Preprocesador::tokenizar(const string &texto) {
    vector<string> tokens;
    stringstream ss(texto);
    string palabra;
    while (ss >> palabra) {
        tokens.push_back(palabra);
    }
    return tokens;
}

void Preprocesador::agregarTokens(DocumentoIndexado &doc, const string &texto, int peso) {
    vector<string> tokens =tokenizar(texto);
    for (const string& t : tokens)
        doc.tokens.push_back({t,peso});
}

DocumentoIndexado Preprocesador::procesarMovie(int movieID, const DataLimpia &movie) {
    DocumentoIndexado doc;
    doc.movieID = movieID;

    agregarTokens(
        doc,
        movie.getTitle(),
        3
    );

    agregarTokens(
        doc,
        movie.getGenre(),
        2
    );

    agregarTokens(
        doc,
        movie.getCast(),
        2
    );

    agregarTokens(
        doc,
        movie.getPlot(),
        1
    );
    return doc;
}

void Preprocesador::preprocesar(const unordered_map<int, DataLimpia> &peliculas) {
    totalDocs = peliculas.size();
    for (const auto& par : peliculas) {
        int movieID = par.first;
        const DataLimpia& movie = par.second;
        DocumentoIndexado doc = procesarMovie(movieID, movie);
        documentosProcesados[movieID] = doc;

        // calcular DOCFREQ (IDF)
        unordered_set<string> vistos;
        for (const TokenInfo& tk : doc.tokens) {
            if (vistos.find(tk.token)==vistos.end()) {
                docFreq[tk.token]++;
                vistos.insert(tk.token);
            }
        }
    }
}
