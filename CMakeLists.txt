cmake_minimum_required(VERSION 4.0)
project(PROYECTAZO)

set(CMAKE_CXX_STANDARD 26)

add_executable(PROYECTAZO main.cpp
        CClases.cpp
        Interfaz.cpp
        LecPelis.cpp
        LimPelis.cpp
        IUsuarios.cpp
        Trie.cpp
        PreProcesador.cpp
        PreProcesador.h
)
