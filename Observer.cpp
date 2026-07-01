#include "Observer.h"

static string fechaHoraActualNotificacion() {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    tm tmBuf;
    localtime_s(&tmBuf, &time);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &tmBuf);
    return buffer;
}

NotificacionesArchivoObserver::NotificacionesArchivoObserver(string archivo)
    : archivo(move(archivo)) {}

EventoAccionUsuario::EventoAccionUsuario(string email, string usuario, string tipo, int movieId, string titulo)
    : email(move(email)),
      usuario(move(usuario)),
      tipo(move(tipo)),
      movieId(movieId),
      titulo(move(titulo)),
      fecha(fechaHoraActualNotificacion()) {}

void NotificacionesArchivoObserver::actualizar(const EventoAccionUsuario& evento) {
    ofstream salida(archivo, ios::app);
    if (!salida.is_open()) {
        return;
    }

    salida << "usuario: " << evento.usuario
           << " | email: " << evento.email
           << " | " << evento.tipo
           << " | id: " << evento.movieId
           << " | titulo: " << evento.titulo
           << " | fecha: " << evento.fecha << "\n";
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
