#include "Proxy.h"

static bool contieneTerminoRestringido(const string& texto) {
    string textoMinuscula = aMinuscula(texto);
    vector<string> terminosRestringidos = {
        "adult",
        "erotic",
        "erotica",
        "erotico",
        "nude",
        "nudity",
        "porn",
        "porno",
        "sex",
        "sexo",
        "sexual",
        "xxx"
    };

    for (const string& termino : terminosRestringidos) {
        if (textoMinuscula.find(termino) != string::npos) {
            return true;
        }
    }
    return false;
}

static bool esContenidoRestringido(const Movie& pelicula) {
    string datos = pelicula.getTtitle() + " " +
                   pelicula.getGenre() + " " +
                   pelicula.getPlot() + " " +
                   pelicula.getCast() + " " +
                   pelicula.getDirector() + " " +
                   pelicula.getOrigin();

    return contieneTerminoRestringido(datos);
}

PeliculaReal::PeliculaReal(const Movie* _pelicula) {
    pelicula = _pelicula;
}

string PeliculaReal::getTitulo() const {
    if (pelicula == nullptr) return "";
    return pelicula->getTtitle();
}

bool PeliculaReal::puedeMostrarse() const {
    return pelicula != nullptr;
}

const Movie* PeliculaReal::getPelicula() const {
    return pelicula;
}

PeliculaProxy::PeliculaProxy(const PeliculaReal* _pelicula, int _edadUsuario) {
    pelicula = _pelicula;
    edadUsuario = _edadUsuario;
}

bool PeliculaProxy::puedeMostrarse() const {
    if (pelicula == nullptr) return false;
    const Movie* datosPelicula = pelicula->getPelicula();
    if (datosPelicula == nullptr) return false;

    bool usuarioMenor = edadUsuario >= 0 && edadUsuario < 18;
    return !usuarioMenor || !esContenidoRestringido(*datosPelicula);
}

string PeliculaProxy::getTitulo() const {
    if (pelicula == nullptr) return "";

    if (!puedeMostrarse()) {
        return "[Contenido restringido]";
    }

    return pelicula->getTitulo();
}
