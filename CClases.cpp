//
// Created by burgo on 27/3/2026.
//

#include "CClases.h"
#include "Pprocesamiento.h"
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

string Movie::getYear()const {return release_year;}
string Movie::getTtitle() const {return title;}
string Movie::getDirector()const  {return director;}
string Movie::getGenre() const {return genre;}
string Movie::getOrigin() const {return origin;}
string Movie::getPlot() const {return plot;}
string Movie::getWiki() const {return wiki_page;}

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

// Integrado con las funciones robustas de tu parser de CSV
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

vector<int> parseLista(const string& s) {
    vector<int> res;
    string num = "";

    for (char c : s) {
        if (isdigit(c)) {
            num += c;
        } else {
            if (!num.empty()) {
                try { res.push_back(stoi(num)); } catch (...) {}
                num = "";
            }
        }
    }
    if (!num.empty()) {
        try { res.push_back(stoi(num)); } catch (...) {}
    }
    return res;
}

unordered_map<int, Movie> convertirAPelis(const vector<int>& ids, const unordered_map<int, Movie>& pelis) {
    unordered_map<int, Movie> res;
    for (int id : ids) {
        auto it = pelis.find(id);
        if (it != pelis.end()) {
            res.insert({id, it->second});
        }
    }
    return res;
}

vector<Usuario> leerUsuarios(const string &csv, const unordered_map<int,Movie>& pelis) {
    vector<Usuario> resultado;
    ifstream archivo(csv);
    if (!archivo.is_open()) {
        cout << "ERROR: no se pudo abrir el archivo de usuarios\n";
        system("pause");
        exit(1);
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string _user, _email, _pass, _vTarde, _likes, _ban, _hist;

        getline(ss, _email, ',');
        getline(ss, _pass, ',');
        getline(ss, _user, ',');

        getline(ss, _vTarde, ']'); _vTarde += "]"; ss.ignore(1);
        getline(ss, _likes, ']'); _likes += "]"; ss.ignore(1);
        getline(ss, _ban, ']'); _ban += "]"; ss.ignore(1);
        getline(ss, _hist, ']'); _hist += "]";

        unordered_map<int,Movie> VT = convertirAPelis(parseLista(_vTarde), pelis);
        unordered_map<int,Movie> MG = convertirAPelis(parseLista(_likes), pelis);
        unordered_map<int,Movie> Ban = convertirAPelis(parseLista(_ban), pelis);
        unordered_map<int,Movie> Hist = convertirAPelis(parseLista(_hist), pelis);

        Usuario us(_user, _email, _pass, VT, MG, Ban, Hist);
        resultado.push_back(us);
    }
    archivo.close();
    return resultado;
}

bool validar_correo(const string& _email) {
    ifstream archivo("registroUsuarios.txt");
    string linea, username, clave, email;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');
        if (email==_email) return true;
    }
    archivo.close();
    return false;
}

bool validar_info(const string& _email, const string& _clave) {
    ifstream archivo("registroUsuarios.txt");
    string linea, username, email, clave;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss,email,',');
        getline(ss,clave,',');
        getline(ss, username, ',');
        if (email==_email and clave==_clave) return true;
    }
    archivo.close();
    return false;
}

void registrar_nuevoUsuario(const string& name, const string& email, const string& clave) {
    ofstream archivo("registroUsuarios.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de usuarios\n";
        return;
    }
    archivo << email << "," << clave << "," << name << ","
            << "[],[],[],[]\n";
    archivo.close();
}

void actualizarUsuario(vector<int> pelis,string tipo){}

vector<string> mostrar_usuarios() {
    ifstream archivo("registroUsuarios.txt");
    string linea, username, email, clave;
    vector<string> usuarios_name;
    if (!archivo.is_open()) return {};

    bool hayDatos = false;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');
        usuarios_name.push_back(username);
        hayDatos = true;
    }
    if (!hayDatos) cout << "No hay usuarios registrados\n";
    archivo.close();
    return usuarios_name;
}

bool validar_usuario(const string& _username) {
    ifstream archivo("registroUsuarios.txt");
    string linea, username, email, clave;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');
        if (username == _username) return true;
    }
    return false;
}

