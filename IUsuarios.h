#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_IUSUARIOS_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_IUSUARIOS_H

#include "Utilidades.h"
#include "Memento.h"
#include "Interfaz.h"
#include "LimPelis.h"


string aMinusculas(string texto);
unordered_map<int, Movie> convertirAPelis(const vector<int>& ids, const unordered_map<int, Movie>& pelis);
vector<Usuario> leerUsuarios(const string &csv, const unordered_map<int,Movie>& pelis);

vector<int> parseLista(const string& s);

struct DatosUsuario {
    string email;
    string clave;
    string fechaNac = "01/01/2000";
    string nombre;
    vector<int> verMasTarde;
    vector<int> likes;
    vector<int> baneados;
    vector<int> historialPeliculas;
};

bool validar_correo(const string& _email);
bool validar_info(const string& _email, const string& _clave);
bool obtener_usuario(const string& email, const string& clave, DatosUsuario& usuario);
int UserEdad(const string& _email, const string& _clave);
string UserFechaNac(const string& _email, const string& _clave);

void registrar_nuevoUsuario(const string& name, const string& fechaNac, const string& email, const string& clave);
void registrarPreguntasRecuperacion(const string& email);
bool restaurarPasswordConPreguntas(const string& email);
bool restaurarPasswordUsuario(const string& email);
bool agregarLikeUsuario(const string& email, int movieId);
bool agregarVerMasTardeUsuario(const string& email, int movieId);
bool agregarFavoritoUsuario(const string& email, int movieId);
bool actualizarNombreUsuario(const string& email, const string& nuevoNombre);
bool actualizarFechaNacimientoUsuario(const string& email, const string& nuevaFechaNac);
bool actualizarPasswordUsuario(const string& email, const string& nuevaPassword);
bool obtener_usuario_por_email(const string& email, DatosUsuario& usuario);
void registrarHistorialUsuario(const string& email, const string& tipo, int movieId, const string& detalle);
vector<HistorialEntry> leerHistorialUsuario(const string& email);
void mostrarHistorialUsuario(const string& email);
void mostrarFavoritosUsuario(const string& email, const unordered_map<int, Movie>& pelis);
stack<string> cargarNotificacionesUsuario(const string& email);
void mostrarNotificacionesUsuario(stack<string> notificaciones);

void peliculasRecomendadasPanel(const unordered_map<int, Movie>& pelis, int edadUsuario = 18);
string UserName(const string& _email, const string& _clave);
vector<string> mostrar_usuarios();


#endif //PROYECTO_1_PROGRAMACION_III_UTEC_IUSUARIOS_H
