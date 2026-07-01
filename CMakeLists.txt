cmake_minimum_required(VERSION 3.20)
project(PROYECTAZO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PROYECTAZO main.cpp
        Utilidades.cpp
        Interfaz.cpp
        LimPelis.cpp
        IUsuarios.cpp
        Procesador.cpp
        Trie.cpp
        Command.cpp
        Iterator.cpp
        Memento.cpp
        Observer.cpp
        Proxy.cpp
        RankingStrategy.cpp
        Singleton.cpp
)
