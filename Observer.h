#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_OBSERVER_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_OBSERVER_H

#include "Utilidades.h"

struct EventoAccionUsuario {
    string tipo;
    int movieId;
    string titulo;
};

class ObservadorAccion {
public:
    virtual ~ObservadorAccion() = default;
    virtual void actualizar(const EventoAccionUsuario& evento) = 0;
};

class NotificacionesArchivoObserver : public ObservadorAccion {
    string archivo;
public:
    explicit NotificacionesArchivoObserver(string archivo = "notificaciones.txt");
    void actualizar(const EventoAccionUsuario& evento) override;
};

class AccionUsuarioSubject {
    vector<ObservadorAccion*> observadores;
public:
    void agregarObservador(ObservadorAccion* observador);
    void notificar(const EventoAccionUsuario& evento);
};

#endif
