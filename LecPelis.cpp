#include "LecPelis.h"

//Registrar toda la información de una pelicula en una sola linea (string)
bool leerFilaCSV(ifstream& file, string& lineaCompleta) {
    string linea;
    if (!getline(file, linea)) return false;

    lineaCompleta = linea;
    int comillas = 0;
    for (char c : linea) { if (c == '"') comillas++; }

    while (comillas % 2 != 0) {
        if (!getline(file, linea)) break;
        lineaCompleta += "\n" + linea;
        for (char c : linea) { if (c == '"') comillas++; }
    }
    return true;
}

//Tratamiento de situaciones particulares con el uso de comillas y registro de atributos
vector<string> parseCSVLine(const string& linea) {
    vector<string> fila;
    string campo = "";
    bool enComillas = false;

    for (size_t i = 0; i < linea.size(); i++) {
        char c = linea[i];
        if (c == '"') {
            if (enComillas && i + 1 < linea.size() && linea[i + 1] == '"') {
                campo += '"'; i++;
            } else {
                enComillas = !enComillas;
            }
        }
        else if (c == ',' && !enComillas) {
            fila.push_back(campo);
            campo = "";
        }
        else { campo += c; }
    }
    fila.push_back(campo);
    return fila;
}


//Lectura del CSV y creacion de todas las peliculas en un unordered_map
unordered_map<int,Movie> leerPeliculas(const string& csv) {
    unordered_map<int,Movie> movies;
    int idMovie = 1;
    ifstream archivo(csv);
    if (!archivo.is_open()) return movies;

    string linea;
    leerFilaCSV(archivo, linea); // Saltar cabecera
    
    while (leerFilaCSV(archivo, linea)) {
        vector<string> fila = parseCSVLine(linea);
        if (fila.size() < 8) continue;
        string _year = fila[0];
        string _title = fila[1];
        string _origin = fila[2];
        string _director = fila[3];
        string _cast = fila[4];
        string _genre = fila[5];
        string _wiki = fila[6];
        string _plot = fila[7];

        movies.insert({idMovie, Movie(_year,_title,_origin,_director,_cast,_genre,_wiki,_plot)});
        idMovie++;
    }
    archivo.close();
    return movies;
}
