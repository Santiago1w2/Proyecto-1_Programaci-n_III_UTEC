//
// Created by burgo on 27/3/2026.
//

#include "CClases.h"

Movie::Movie(int _id, string _year,string _title, string _origin,string _director ,string _cast, string _genre, string wiki, string _plot) {
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

int Movie::getId() {return id;}
string Movie::getYear() {return release_year;}
string Movie::getTtitle() {return title;}
string Movie::getDirector() {return director;}
string Movie::getGenre() {return genre;}
string Movie::getOrigin() {return origin;}
string Movie::getPlot() {return plot;}
string Movie::getWiki() {return wiki_page;}

void procesarComillas(stringstream& ss,string& name) {
    char c;
    if (!ss.get(c)) return;
    if (c=='"') {
        string compuesto="";
        char nextChar;
        while (ss.get(nextChar)) {
            if (nextChar=='"') {
                if (ss.peek()=='"') {
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
    int idMovie = 1;
    ifstream archivo(csv);
    string linea;
    string _year,_title,_origin,_director,_cast,_genre,_wiki,_plot;
    //No queremos leer las cabeceras de las columnas
    getline(archivo, linea);
    while (true) {
        string registro;
        int comillas = 0;
        if (!getline(archivo, linea)) break;
        //Para la primera linea no queremos agregar un salto de linea
        registro += linea;
        for (char c: linea)
            if (c=='"')
                comillas++;
        while (comillas % 2 != 0) {
            if (!getline(archivo, linea)) break;
            registro += "\n" + linea;
            for (char c: linea)
                if (c=='"')
                    comillas++;
        }
        stringstream ss(registro);
        getline(ss,_year,',');
        procesarComillas(ss,_title);
        procesarComillas(ss,_origin);
        procesarComillas(ss,_director);
        procesarComillas(ss,_cast);
        procesarComillas(ss,_genre);
        procesarComillas(ss,_wiki);
        getline(ss,_plot);
        movies.push_back(Movie(idMovie,_year,_title,_origin,_director,_cast,_genre,_wiki,_plot));
        idMovie++;
    }
    return movies;
}

bool validar_correo(const string& _email) {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string email;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss,email,',');
        if (email==_email)
            return true;
    }
    return false;
}


bool validar_info(const string& _email, const string& _clave) {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string email,clave;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss,email,',');
        getline(ss,clave,',');
        if (email==_email and clave==_clave)
            return true;
    }
    return false;
}

