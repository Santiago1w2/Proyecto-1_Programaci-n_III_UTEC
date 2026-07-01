#include "Command.h"

CambiarNombreCommand::CambiarNombreCommand(string email, string nuevoNombre)
    : email(move(email)), nuevoNombre(move(nuevoNombre)) {}

bool CambiarNombreCommand::ejecutar() {
    return actualizarNombreUsuario(email, nuevoNombre);
}

CambiarFechaNacimientoCommand::CambiarFechaNacimientoCommand(string email, string nuevaFechaNac)
    : email(move(email)), nuevaFechaNac(move(nuevaFechaNac)) {}

bool CambiarFechaNacimientoCommand::ejecutar() {
    return actualizarFechaNacimientoUsuario(email, nuevaFechaNac);
}

CambiarPasswordCommand::CambiarPasswordCommand(string email, string nuevaPassword)
    : email(move(email)), nuevaPassword(move(nuevaPassword)) {}

bool CambiarPasswordCommand::ejecutar() {
    return actualizarPasswordUsuario(email, nuevaPassword);
}
