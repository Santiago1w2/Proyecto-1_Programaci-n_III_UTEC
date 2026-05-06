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

int buscarPeli(const map<int, Movie>& v, int objetivo_id) {
    auto it = v.find(objetivo_id);
    if (it != v.end()) return it->first;
    return -1;
}

vector<string> separar(const string& texto) {
    string procesado = aMinusculas(texto);
    stringstream ss(procesado);
    string palabra;
    vector<string> palabras;
    while (ss >> palabra) {
        palabras.push_back(palabra);
    }
    return palabras;
}

Trie::Trie() { raiz = new Nodo; }

// keywordIndex: mapa palabra -> set de IDs
// Se usa para busqueda por SUBCADENA (ej: "ert" encuentra "puerta")
// El Trie cubre palabras completas y prefijos; este indice cubre lo demas
unordered_map<string, vector<int>> keywordIndex;

void Trie::insertarCompleto(const string& texto, int id, int pesoCampo) {
    stringstream ss(texto);
    string palabra;

    while (ss >> palabra) {

        int n = palabra.size();

        // filtro
        if (n < 3) continue;

        //palabra completa prefijos
        {
            Nodo* nodo = raiz;
            for (char c : palabra) {
                if (!nodo->hijos[c]) nodo->hijos[c] = new Nodo();
                nodo = nodo->hijos[c];
            }
            nodo->esFinDePalabra = true;
            nodo->freq[id] += pesoCampo;
        }

        // sufijos substrings
        for (int i = 1; i < n; i++) {

            int len = n - i;

            //filtro
            if (len < 3) continue;

            Nodo* nodo = raiz;

            for (int j = i; j < n; j++) {
                char c = palabra[j];
                if (!nodo->hijos[c]) nodo->hijos[c] = new Nodo();
                nodo = nodo->hijos[c];
            }

            nodo->esFinDePalabra = true;

            // el peor tien que ser menor a a plabra completa
            nodo->freq[id] += pesoCampo / 2;
        }
    }
}

void construirIndice(const unordered_map<int, string>& dataLimpia) {
    for (const auto& [id, texto] : dataLimpia) {
        stringstream ss(texto);
        string palabra;
        unordered_set<string> vistas;
        while (ss >> palabra) {
            if (palabra.size() < 2) continue; // si la plabaras es menor que 1 no se agrega ya que es basura
            if (!vistas.insert(palabra).second) continue; // no duplicar por pelicula
            keywordIndex[palabra].push_back(id);
        }
    }
    cout << "Indice construido: " << keywordIndex.size() << " palabras unicas.\n";
}

// DFS: recorre hijos del nodo y acumula IDs en score
// Sirve para prefijos: buscar "car" encuentra "carro", "carbon", etc.
// total de pelis

void Trie::construirIndice(const unordered_map<int, string>& data) {
    totalDocs = data.size();

    for (auto& [id, texto] : data) {
        stringstream ss(texto);
        string palabra;

        unordered_set<string> vistas;

        while (ss >> palabra) {
            // evitar contar repetido en misma peli
            if (vistas.insert(palabra).second) {
                docFreq[palabra]++;
            }
        }
    }
}
vector<int> Trie::buscar(const string& query) {
    unordered_map<int, double> score;

    stringstream ss(query);
    string palabra;

    while (ss >> palabra) {
        Nodo* nodo = raiz;

        // bajar en el Trie
        for (char c : palabra) {
            if (!nodo->hijos.count(c)) {
                nodo = nullptr;
                break;
            }
            nodo = nodo->hijos[c];
        }

        if (!nodo || !nodo->esFinDePalabra) continue;

        // 🔥 calcular IDF
        int df = docFreq[palabra];
        double idf = log((double)totalDocs / (1 + df));

        // 🔥 aplicar TF-IDF
        for (auto& [id, tf] : nodo->freq) {
            score[id] += tf * idf;
        }
    }

    // ordenar
    vector<pair<int,double>> orden(score.begin(), score.end());

    sort(orden.begin(), orden.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    vector<int> res;
    for (int i = 0; i < min(5, (int)orden.size()); i++) {
        res.push_back(orden[i].first);
    }

    return res;
}
void peliculasRecomendadas(const string &_email,const vector<Movie>& pelis) {
    vector<int> hist;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, pelis.size());
    bool cond2=false;
    int id;

    for (int i=0;i<10;i++) {
        id = dist(gen);
        bool cond1 = find(hist.begin(),hist.end(),id)==hist.end();
        if (i!=0)
            cond2 = pelis[id-1].getGenre()==pelis[hist[i-1]-1].getGenre();

        while (cond1==true and cond2==true) {
            id = dist(gen);
            cond1 = find(hist.begin(),hist.end(),id)==hist.end();
            cond2 = pelis[id-1].getGenre()==pelis[hist[i-1]-1].getGenre();
        }
        hist.push_back(id);
    }

    cout<<left;
    cout<<setw(10)<<"OPCION"<<setw(49)<<"TITULO "<<setw(20)<<"GENERO"<<setw(20)<<"LANZAMIENTO"<<endl;
    cout<<string(100,'-')<<endl;
    for (int i=0;i<hist.size();i++) {
        cout<<setw(10)<<to_string(i+1)+"-> ";
        cout<<setw(50)<<pelis[hist[i]-1].getTtitle();
        cout<<setw(20)<<pelis[hist[i]-1].getGenre();
        cout<<setw(20)<<pelis[hist[i]-1].getYear()<<endl;
    }
}