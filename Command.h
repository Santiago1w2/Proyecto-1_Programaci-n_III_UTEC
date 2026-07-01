#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_COMMAND_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_COMMAND_H

#include "IUsuarios.h"

class PerfilCommand {
public:
    virtual ~PerfilCommand() = default;
    virtual bool ejecutar() = 0;
};

class CambiarNombreCommand : public PerfilCommand {
    string email;
    string nuevoNombre;
public:
    CambiarNombreCommand(string email, string nuevoNombre);
    bool ejecutar() override;
};

class CambiarFechaNacimientoCommand : public PerfilCommand {
    string email;
    string nuevaFechaNac;
public:
    CambiarFechaNacimientoCommand(string email, string nuevaFechaNac);
    bool ejecutar() override;
};

class CambiarPasswordCommand : public PerfilCommand {
    string email;
    string nuevaPassword;
public:
    CambiarPasswordCommand(string email, string nuevaPassword);
    bool ejecutar() override;
};

#endif
