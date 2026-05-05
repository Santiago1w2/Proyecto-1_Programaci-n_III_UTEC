cmake_minimum_required(VERSION 4.0)
project(PROYECTAZO)

set(CMAKE_CXX_STANDARD 26)

add_executable(PROYECTAZO main.cpp
        CClases.h
        CClases.cpp
        Interfaz.cpp
        Interfaz.h
        Pprocesamiento.h
        Pprocesamiento.cpp
)
