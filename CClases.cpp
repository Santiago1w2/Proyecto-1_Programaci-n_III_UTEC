//
// Created by burgo on 27/3/2026.
//

#include "CClases.h"

Movie::Movie(int _id,string _title, string _year, string _origin,string _director ,string _cast, string _genre, string wiki, string _plot) {
    id = _id;
    title = _title;
    release_year = _year;
    origin = _origin;
    director = _director;
    cast = _cast;
    genre = _genre;
    wiki_page = wiki;
    plot = _plot;
}

void procesarConComas(stringstream& ss,string& name) {
    char c;
    ss.get(c);
    if (c=='"') {
        string compuesto="";
        char nextChar;
        while (ss.get(nextChar)) {
            if (nextChar=='"') {
                if (ss.peek()=='"') { //Se puede considerar innecesario pero nos aseguramos que el csv este bien definido
                    ss.get();
                    compuesto+='"';
                }
                else if (ss.peek()==',') {
                    ss.get();
                    break;
                }
            }
            else
                compuesto+=nextChar;
        }
        name = compuesto;
    }
    else {
        ss.unget();
        getline(ss,name,',');
    }
}


//Procesamiento de datos previo al uso de arboles
vector<Movie> leerCSV(const string& csv) {
    vector<Movie> movies;
    int idMovie=0;
    ifstream archivo(csv);
    string linea;
    string _year,_title,_origin,_director,_cast,_genre,_wiki,_plot;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        string title,director,cast,year,description;
        getline(ss,_year,',');
        procesarConComas(ss,_title);
        procesarConComas(ss,_origin);
        procesarConComas(ss,_director);
        procesarConComas(ss,_cast);
        procesarConComas(ss,_genre);
        procesarConComas(ss,_wiki);
        procesarConComas(ss,_plot);
        movies.push_back(Movie(idMovie,_year,_title,_origin,_director,_cast,_genre,_wiki,_plot));
        idMovie++;
    }
    return movies;
}



