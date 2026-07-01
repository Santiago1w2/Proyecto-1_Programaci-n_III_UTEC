#include "Proxy.h"

PeliculaReal::PeliculaReal(const Movie* _movie) : movie(_movie) {}

string PeliculaReal::getTitulo() const {
    return movie ? movie->getTtitle() : "";
}

string PeliculaReal::getGenero() const {
    return movie ? movie->getGenre() : "";
}

string PeliculaReal::getPlot() const {
    return movie ? movie->getPlot() : "";
}

bool PeliculaReal::esRestringida() const {
    string genero = aMinuscula(getGenero());
    string plot = aMinuscula(getPlot());
    return genero.find("adult") != string::npos ||
           genero.find("horror") != string::npos ||
           genero.find("crime") != string::npos ||
           plot.find("murder") != string::npos ||
           plot.find("violence") != string::npos;
}

PeliculaProxy::PeliculaProxy(const IPelicula* _pelicula, int _edadUsuario)
    : pelicula(_pelicula), edadUsuario(_edadUsuario) {}

string PeliculaProxy::getTitulo() const {
    if (!pelicula) {
        return "";
    }
    if (esRestringida() && edadUsuario < 18) {
        return "[Contenido restringido]";
    }
    return pelicula->getTitulo();
}

string PeliculaProxy::getGenero() const {
    if (!pelicula) {
        return "";
    }
    if (esRestringida() && edadUsuario < 18) {
        return "restringido";
    }
    return pelicula->getGenero();
}

string PeliculaProxy::getPlot() const {
    if (!pelicula) {
        return "";
    }
    if (esRestringida() && edadUsuario < 18) {
        return "Contenido disponible solo para usuarios mayores de edad.";
    }
    return pelicula->getPlot();
}

bool PeliculaProxy::esRestringida() const {
    return pelicula && pelicula->esRestringida();
}
