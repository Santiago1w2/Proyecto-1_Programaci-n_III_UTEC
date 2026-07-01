#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_AUTENTICACIONSTRATEGY_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_AUTENTICACIONSTRATEGY_H

#include "Utilidades.h"

class ContextoAutenticacion;

struct UsuarioAutenticado {
    bool autenticado = false;
    string email;
    string password;
    string nombre;
    string fechaNac;
};

class AutenticacionStrategy {
public:
    virtual ~AutenticacionStrategy() = default;
    virtual UsuarioAutenticado autenticar(ContextoAutenticacion& contexto) = 0;
};

class ContextoAutenticacion {
    unique_ptr<AutenticacionStrategy> estrategia;
    unique_ptr<AutenticacionStrategy> siguienteEstrategia;
    const unordered_map<int, Movie>& peliculas;
    string correoActual;
public:
    explicit ContextoAutenticacion(const unordered_map<int, Movie>& _peliculas);

    UsuarioAutenticado iniciar();
    void cambiarEstrategia(unique_ptr<AutenticacionStrategy> nuevaEstrategia);

    const unordered_map<int, Movie>& getPeliculas() const;
    string getCorreoActual() const;
    void setCorreoActual(const string& correo);
};

class AutenticacionContrasenaStrategy : public AutenticacionStrategy {
public:
    UsuarioAutenticado autenticar(ContextoAutenticacion& contexto) override;
};

class AutenticacionPreguntasStrategy : public AutenticacionStrategy {
public:
    UsuarioAutenticado autenticar(ContextoAutenticacion& contexto) override;
};

class AutenticacionHistorialStrategy : public AutenticacionStrategy {
public:
    UsuarioAutenticado autenticar(ContextoAutenticacion& contexto) override;
};

void registrarPreguntasRecuperacion(const string& email);

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_AUTENTICACIONSTRATEGY_H
