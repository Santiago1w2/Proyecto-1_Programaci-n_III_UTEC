#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_PROXY_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_PROXY_H

#include "Utilidades.h"

class IPelicula {
public:
    virtual ~IPelicula() = default;
    virtual string getTitulo() const = 0;
    virtual string getGenero() const = 0;
    virtual string getPlot() const = 0;
    virtual bool esRestringida() const = 0;
};

class PeliculaReal : public IPelicula {
    const Movie* movie;
public:
    explicit PeliculaReal(const Movie* _movie);
    string getTitulo() const override;
    string getGenero() const override;
    string getPlot() const override;
    bool esRestringida() const override;
};

class PeliculaProxy : public IPelicula {
    const IPelicula* pelicula;
    int edadUsuario;
public:
    PeliculaProxy(const IPelicula* _pelicula, int _edadUsuario);
    string getTitulo() const override;
    string getGenero() const override;
    string getPlot() const override;
    bool esRestringida() const override;
};

#endif
