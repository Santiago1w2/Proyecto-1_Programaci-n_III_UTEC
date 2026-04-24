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
};

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
    string username,clave,email;
    while (getline(archivo,linea)) {

        stringstream ss(linea);

        getline(ss, username, ',');
        getline(ss, email, ',');
        getline(ss, clave, ',');
        if (email==_email)
            return true;
    }
    return false;
}


bool validar_info(const string& _email, const string& _clave) {
    ifstream archivo("registroUsuarios.txt");
    string linea;
    string username,email,clave;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss, username, ',');
        getline(ss,email,',');
        getline(ss,clave,',');
        if (email==_email and clave==_clave)
            return true;
    }
    return false;
}
void registrar_usuario(const string& name, const string& email, const string& clave) {
    ofstream archivo("registroUsuarios.txt", ios::app); //
    if (archivo.is_open()) {
        archivo << name << "," << email<<"," << clave << "\n";
        archivo.close();
    }
}


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

        getline(ss, username, ',');
        getline(ss, email, ',');
        getline(ss, clave, ',');

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

        getline(ss, username, ',');
        getline(ss, email, ',');
        getline(ss, clave, ',');

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
Arbol::Arbol() {
    raiz = new Nodo;
}

void Arbol::insertar(Movie nuevo) {
    Nodo* nodo = raiz;
    for (char c: nuevo.getTtitle()) {
        if (nodo->nodos.find(c) == nodo->nodos.end()) {
            nodo->nodos[c]=new Nodo(); // verifica si son iguales, sino parte a hacer otro nodo
            //ejemplo:
                 /*

                          r - r - o
            Arbol -- c - a
                          s - a

                  */
        }
        nodo = nodo->nodos[c];

        // se guarda a peliculas
        nodo->peliculas.push_back(nuevo);
    }
}

vector<Movie> Arbol::buscar1(string palabra) {
    Nodo* nodo = raiz;
    for (char c: palabra) {
        if (nodo->nodos.find(c) == nodo->nodos.end()) {
            return {}; // no se encontro
        }
        nodo = nodo->nodos[c];
    }
    return nodo->peliculas;
}


// | titulo | cast | genre | plot
void peliculasRecomendadas(const string &_email,const vector<Movie>& pelis) {
    //caso en el que no tenga nada recomendado
    vector<int> hist; //Guardamos los resultados
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1,34886);
    bool cond2=false;
    int id;
    for (int i=0;i<10;i++) {
        id = dist(gen);
        bool cond1 =find(hist.begin(),hist.end(),id)==hist.end();
        if (i!=0)
            cond2 = pelis[id-1].getGenre()==pelis[hist[i-1]].getGenre();
        while (cond1==true and cond2==true) {
            id = dist(gen);
            cond1 =find(hist.begin(),hist.end(),id)==hist.end();
            cond2 = pelis[id-1].getGenre()==pelis[hist[i-1]].getGenre();
        }
        hist.push_back(id);
    }
    cout<<left;
    cout<<setw(10)<<"OPCION"<<setw(49)<<"TITULO "<<setw(20)<<"GENERO"<<setw(20)<<"LANZAMIENTO"<<endl;
    cout<<string(100,'-')<<endl;
    for (int i=0;i<hist.size();i++) {
        cout<<setw(10)<<to_string(i+1)+"-> ";
        cout<<setw(50)<<pelis[hist[i]].getTtitle();
        cout<<setw(20)<<pelis[hist[i]].getGenre();
        cout<<setw(20)<<pelis[hist[i]].getYear()<<endl;
    }
}
