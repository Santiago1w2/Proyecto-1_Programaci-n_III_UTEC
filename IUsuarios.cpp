#include "IUsuarios.h"

//Flujo(funciones) para la interaccion entre el programa y el usuario


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

//Se utiliza para las peliculas de los usuarios (Likes,Ban,etc)
//Busca los 'ids' registrados y los guarda en un mapa
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


//Se utiliza cuando un usuario nuevo se va a registrar
//Verifica que el correo que esta intentando ingresar no esta registrado
//Si lo tomamos como una tabla en una base de datos, el correo es la llave primaria
bool validar_correo(const string& _email) {
    ifstream archivo("registroUsuarios.txt"); //registroUsuarios manteniene un historial de todos los usuarios registrados
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

//Se utiliza para usuarios que ya estan registrados
//Valida que las credenciales que estan ingresando sean iguales a las registradas
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


//Se utiliza para obtener el nombre del usuario correspondiente a las credenciales ingresadas (cuando ya se ha validado)
string UserName(const string& _email, const string& _clave) {
    ifstream archivo("registroUsuarios.txt");
    string linea, username = "", email, clave;
    while (getline(archivo,linea)) {
        stringstream ss(linea);
        getline(ss,email,',');
        getline(ss,clave,',');
        getline(ss, username, ',');
        if (email==_email and clave==_clave) return username;
    }
    archivo.close();
    return username;
}

//Luego de realizar la validación del nuevo usuario (correo no repetido), se registra la información en el historial de usuarios.
void registrar_nuevoUsuario(const string& name, const string& email, const string& clave) {
    ofstream archivo("registroUsuarios.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir archivo de usuarios\n";
        return;
    }
    archivo << email << "," << clave << "," << name <<","<< "[],[],[],[]\n";
    archivo.close();
}


//Fncion utilizada par visualizar a los usuarios registrados (seria adecuado para un rol admin)
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



//Recomendacion aleatoria de 5 peliculas
void peliculasRecomendadasPanel(const unordered_map<int, Movie>& pelis) {
    //Historial temporal de peliculas que se estan recomendando en una misma interacción
    unordered_map<int,string> hist_temp;

    random_device rd;
    mt19937 gen(rd());
    int n=pelis.size();
    if (n==0) return; //Queremos que la funcion sea lo mas escalable posible
    uniform_int_distribution<> dist(0, n - 1);
    int id;
    string genre;

    // Algoritmo para la recomendacion aletoria de peliculas
    for (int i=0;i<5;i++) {
        bool cond1 = false;
        bool cond2 = false; //Es necesario que se inicialice como false ya que no se aplica para la primera pelicula (i=0)
        do {
            id = dist(gen);

            //No queremos que se recomiende una pelicula más de una vez en una misma interaccion.
            //Se verifica que el ID de la pelicula no este registrado en el historial
            cond1 = hist_temp.find(id)==hist_temp.end();

            //Se busca recomendar peliculas de diferente genero.
            auto it = pelis.find(id);
            genre = it->second.getGenre();
            cond2 = false;
            if (i != 0)
                for (auto& e:hist_temp)
                    if (e.second==genre) {
                        cond2 = true;
                        break;
                    }
            //COND1 -> TRUE: ID aun no esta registrado
            //COND2 -> TRUE: Ya se ha registrado una pelicula con el mismo genero
        } while (cond1==false || cond2==true);
        hist_temp[id] = genre;
    }

    // Algoritmo para la impresion de las recomendaciones
    int fila = 8;
    int col = 45;
    for (auto& k:hist_temp) {
        const Movie& m = pelis.at(k.first);
        string titulo = m.getTtitle();
        if (titulo.size() > 25)
            titulo = titulo.substr(0, 25);
        moverCursor(col, fila);
        cout << "[" << k.first << "] " << titulo;
        cout << " " << m.getGenre() << " | " << m.getYear();
        fila++;
    }
}