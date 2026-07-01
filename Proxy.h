#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_PROXY_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_PROXY_H

#include "Utilidades.h"

class IPelicula {
public:
    virtual ~IPelicula() = default;
    virtual string getTitulo() const = 0;
    virtual string getGenero() const = 0;
    virtual string getAnio() const = 0;
    virtual bool puedeMostrarse() const = 0;
};

class PeliculaReal : public IPelicula {
    const Movie* pelicula;
public:
    explicit PeliculaReal(const Movie* pelicula);
    string getTitulo() const override;
    string getGenero() const override;
    string getAnio() const override;
    bool puedeMostrarse() const override;
    const Movie* getPelicula() const;
};

class PeliculaProxy : public IPelicula {
    const PeliculaReal* pelicula;
    int edadUsuario;
public:
    PeliculaProxy(const PeliculaReal* pelicula, int edadUsuario);
    string getTitulo() const override;
    string getGenero() const override;
    string getAnio() const override;
    bool puedeMostrarse() const override;
};

#endif
