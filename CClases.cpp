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

Usuario::Usuario(const string &user, const string &_email, const string &_pass, const vector<Movie> &VT, const vector<Movie> &MG, const vector<Movie> &Ban, const vector<Movie> &hist) {
    username=user;
    email = _email;
    password = _pass;
    verMasTarde = VT;
    meGusta = MG;
    baneado = Ban;
    historial = hist;
}

int Movie::getId() const {return id;}
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
vector<Movie> leerPeliculas(const string& csv) {
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
                try {
                    res.push_back(stoi(num));
                } catch (...) {}
                num = "";
            }
        }
    }

    if (!num.empty()) {
        try {
            res.push_back(stoi(num));
        } catch (...) {}
    }

    return res;
}

vector<Movie> convertirAPelis(const vector<int>& ids, const vector<Movie>& pelis) {
    vector<Movie> res;

    for (int id : ids) {
        if (id >= 0 && id < pelis.size()) {
            res.push_back(pelis[id]); // usa índice directo (0-based)
        }
    }

    return res;
}

vector<Usuario> leerUsuarios(const string &csv, const vector<Movie>& pelis) {
    vector<Usuario> resultado;

    ifstream archivo(csv);
    if (!archivo.is_open()) {
        cout << "ERROR: no se pudo abrir el archivo de usuarios\n";
        system("pause");
        exit(1);
    }

    string linea;
    getline(archivo, linea); // saltar cabecera

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);

        string _user, _email, _pass;
        string _vTarde, _likes, _ban, _hist;

        // datos básicos
        getline(ss, _email, ',');
        getline(ss, _pass, ',');
        getline(ss, _user, ',');

        // leer listas completas tipo [1,2,3]
        getline(ss, _vTarde, ']'); _vTarde += "]";
        ss.ignore(1);

        getline(ss, _likes, ']'); _likes += "]";
        ss.ignore(1);

        getline(ss, _ban, ']'); _ban += "]";
        ss.ignore(1);

        getline(ss, _hist, ']'); _hist += "]";

        // convertir a vectores de películas
        vector<Movie> VT = convertirAPelis(parseLista(_vTarde), pelis);
        vector<Movie> MG = convertirAPelis(parseLista(_likes), pelis);
        vector<Movie> Ban = convertirAPelis(parseLista(_ban), pelis);
        vector<Movie> Hist = convertirAPelis(parseLista(_hist), pelis);

        Usuario us(_user, _email, _pass, VT, MG, Ban, Hist);
        resultado.push_back(us);
    }

    archivo.close();
    return resultado;
}


//True = correo ya registrado /
bool validar_correo(const string& _email) {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string username,clave,email;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');
        if (email==_email)
            return true;
    }
    archivo.close();
    return false;
}

//True = Email y contraseña coinciden coinciden con las registradas
bool validar_info(const string& _email, const string& _clave) {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string username,email,clave;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss,email,',');
        getline(ss,clave,',');
        getline(ss, username, ',');
        if (email==_email and clave==_clave)
            return true;
    }
    archivo.close();
    return false;
}


//Funcion solo para registrar a nuevos usuarios
void registrar_nuevoUsuario(const string& name, const string& email, const string& clave) {
    ofstream archivo("registroUsuarios.txt", ios::app);

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de usuarios\n";
        return;
    }
    archivo << email << ","
            << clave << ","
            << name << ","
            << "[],"
            << "[],"
            << "[],"
            << "[]"
            << "\n";

    archivo.close();
}


void actualizarUsuario(vector<int> pelis,string tipo){}



vector<string> mostrar_usuarios() {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string username, email, clave;
    vector<string> usuarios_name;
    // verificar si el archivo abre
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo\n";
        return {};
    }
    bool hayDatos = false;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue; // aca se vana  ignorar la slineas vacias
        stringstream ss(linea);
        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');
        usuarios_name.push_back(username);
        hayDatos = true;
    }
    if (!hayDatos) {
        cout << "No hay usuarios registrados\n";
    }
    archivo.close();
    return usuarios_name;
}



bool validar_usuario(const string& _username) {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string username, email, clave;

    while (getline(archivo, linea)) {
        stringstream ss(linea);

        getline(ss, email, ',');
        getline(ss, clave, ',');
        getline(ss, username, ',');

        if (username == _username)
            return true;
    }
    return false;
}



int busquedaBinaria(const vector<Movie>& v, int objetivo_id) {
    int left = 0;
    int right = v.size() - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (v[mid].getId() == objetivo_id) {
            return mid; // encontrado
        }
        else if (v[mid].getId() < objetivo_id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1; // no encontrado
}

//Estructura del arbol prueba
vector<string> separar(const string& texto) {
    stringstream ss(texto);
    string palabra;
    vector<string> palabras;

    while (ss >> palabra) {
        palabras.push_back(palabra);
    }

    return palabras;
}
Trie::Trie() {
    raiz = new Nodo;
}

void Trie::insertar(const string& info, int id) {
    Nodo* nodo = raiz;
    vector<string> palabras = separar(info);
    for (const string& palabra : palabras) {
        nodo = raiz; // reinicia todo cada palabra empieza desde la raíz
        for (char c : palabra) {

            if (nodo->hijos.find(c) == nodo->hijos.end()) { // verifica si es no hay camino en ese nodo, find de vuelce end() si no encuentra
                nodo->hijos[c] = new Nodo(); // si no hay se crea
            }
            nodo = nodo->hijos[c]; // se mueve el nodo a la siguiente char
            if (find(nodo->movieIds.begin(), nodo->movieIds.end(), id) == nodo->movieIds.end()) {
                nodo->movieIds.push_back(id);
            }
        }
        nodo->esFinDePalabra= true; // se verifica si ese nodo es el final de una plabra
    }
}

vector<int> Trie::buscar(const string& query) {
    vector<string> palabras = separar(query);
    vector<int> resultadoFinal;
    bool primera = true;
    for (const string& palabra : palabras) {
        Nodo* nodo = raiz;
        for (char c : palabra) {
            if (nodo->hijos.find(c) == nodo->hijos.end()) {
                return {}; // no existe palabra
            }
            nodo = nodo->hijos[c];
        }
        vector<int> ids = nodo->movieIds;
        if (primera) { // si es la primera palabra
            resultadoFinal = ids; //se copia
            primera = false;
        } else { // sino se crea un interseccion para ver si tiene 
            vector<int> temp;

            for (int x : resultadoFinal) {
                for (int y : ids) {
                    if (x == y) {
                        temp.push_back(x);
                    }
                }
            }
            resultadoFinal = temp;
        }
    }

    return resultadoFinal;
}


// | titulo | cast | genre | plot
void peliculasRecomendadas(const string &_email,const vector<Movie>& pelis) {
    //caso en el que no tenga nada recomendado
    vector<int> hist; //Guardamos los resultados
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, pelis.size());
    bool cond2=false;
    int id;
    for (int i=0;i<10;i++) {
        id = dist(gen);
        bool cond1 =find(hist.begin(),hist.end(),id)==hist.end();
        if (i!=0)
            cond2 = pelis[id-1].getGenre()==pelis[hist[i-1]-1].getGenre();
        while (cond1==true and cond2==true) {
            id = dist(gen);
            cond1 =find(hist.begin(),hist.end(),id)==hist.end();
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
