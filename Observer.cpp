#include "Observer.h"

NotificacionesArchivoObserver::NotificacionesArchivoObserver(string archivo)
    : archivo(move(archivo)) {}

void NotificacionesArchivoObserver::actualizar(const EventoAccionUsuario& evento) {
    ofstream salida(archivo, ios::app);
    if (!salida.is_open()) {
        return;
    }

    salida << evento.tipo << " | id: " << evento.movieId
           << " | titulo: " << evento.titulo << "\n";
}

void AccionUsuarioSubject::agregarObservador(ObservadorAccion* observador) {
    if (observador != nullptr) {
        observadores.push_back(observador);
    }
}

void AccionUsuarioSubject::notificar(const EventoAccionUsuario& evento) {
    for (ObservadorAccion* observador : observadores) {
        observador->actualizar(evento);
    }
}
