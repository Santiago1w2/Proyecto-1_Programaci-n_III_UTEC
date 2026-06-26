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


Usuario::Usuario(string &user, string &_email, string &_fechaNac ,string &pass, unordered_map<int,Movie> &VMT,
    unordered_map<int,Movie> &MG, unordered_map<int,Movie> &Ban, unordered_map<int,Movie> &hist) {
    username=user;
    email = _email;
    fechaNac = _fechaNac;
    password = pass;
    verMasTarde = VMT;
    meGusta = MG;
    baneado = Ban;
    historial = hist;
}

string Usuario::getEdad() const {
    return to_string(calcularEdad(fechaNac));
}


static bool esBisiesto(int anio) {
    return (anio % 400 == 0) || (anio % 4 == 0 && anio % 100 != 0);
}

static int diasDelMes(int mes, int anio) {
    static const int dias[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (mes == 2 && esBisiesto(anio)) return 29;
    if (mes < 1 || mes > 12) return 0;
    return dias[mes];
}

int calcularEdad(const string& fechaNac) {
    int dia, mes, anio;
    char sep;
    stringstream ss(fechaNac);
    ss >> dia >> sep >> mes >> sep >> anio;

    SYSTEMTIME hoy;
    GetLocalTime(&hoy);

    if (mes < 1 || mes > 12 || dia < 1 || dia > diasDelMes(mes, anio))
        return -1;

    int edad = hoy.wYear - anio;

    int diaCumple = dia;
    int mesCumple = mes;
    if (mesCumple == 2 && diaCumple == 29 && !esBisiesto(hoy.wYear)) {
        diaCumple = 28;
    }

    if (mesCumple > hoy.wMonth || (mesCumple == hoy.wMonth && diaCumple > hoy.wDay)) {
        edad--;
    }
    return edad;
}

//Convierte un string a minsculas
string aMinuscula(string texto){
    for(char& c : texto) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return texto;
}
