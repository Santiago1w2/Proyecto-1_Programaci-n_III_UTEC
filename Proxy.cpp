#include "Proxy.h"

static bool contieneTerminoRestringido(const string& texto) {
    const string textoMinuscula = aMinuscula(texto);
    const vector<string> terminosRestringidos = {
        "adult", "erotic", "erotica", "erotico", "nude", "nudity",
        "porn", "porno", "sex", "sexo", "sexual", "xxx"
    };

    for (const string& termino : terminosRestringidos) {
        if (textoMinuscula.find(termino) != string::npos) {
            return true;
        }
    }
    return false;
}

static bool esContenidoRestringido(const Movie& pelicula) {
    const string datos = pelicula.getTtitle() + " " +
                         pelicula.getGenre() + " " +
                         pelicula.getPlot() + " " +
                         pelicula.getCast() + " " +
                         pelicula.getDirector() + " " +
                         pelicula.getOrigin();
    return contieneTerminoRestringido(datos);
}

PeliculaReal::PeliculaReal(const Movie* pelicula) : pelicula(pelicula) {}

string PeliculaReal::getTitulo() const {
    return pelicula ? pelicula->getTtitle() : "";
}

string PeliculaReal::getGenero() const {
    return pelicula ? pelicula->getGenre() : "";
}

string PeliculaReal::getAnio() const {
    return pelicula ? pelicula->getYear() : "";
}

bool PeliculaReal::puedeMostrarse() const {
    return pelicula != nullptr;
}

const Movie* PeliculaReal::getPelicula() const {
    return pelicula;
}

PeliculaProxy::PeliculaProxy(const PeliculaReal* pelicula, int edadUsuario)
    : pelicula(pelicula), edadUsuario(edadUsuario) {}

bool PeliculaProxy::puedeMostrarse() const {
    if (pelicula == nullptr || pelicula->getPelicula() == nullptr) {
        return false;
    }

    const bool usuarioMenor = edadUsuario >= 0 && edadUsuario < 18;
    return !usuarioMenor || !esContenidoRestringido(*pelicula->getPelicula());
}

string PeliculaProxy::getTitulo() const {
    if (!puedeMostrarse()) {
        return "[Contenido restringido]";
    }
    return pelicula->getTitulo();
}

string PeliculaProxy::getGenero() const {
    if (!puedeMostrarse()) {
        return "restringido";
    }
    return pelicula->getGenero();
}

string PeliculaProxy::getAnio() const {
    if (!puedeMostrarse()) {
        return "";
    }
    return pelicula->getAnio();
}
