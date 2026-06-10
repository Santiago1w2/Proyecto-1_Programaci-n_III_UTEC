#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H

#include "CClases.h"

inline constexpr int MAX_LONG = 6;



struct Nodo {
    array<Nodo*,256> hijos{};
    unordered_map<int, int> freq;
    bool esPalabraExacta= false;
    Nodo(){hijos.fill(nullptr);}
};


struct Resultado {
    int id;
    double score;
};


class Trie {
    Nodo* raiz;
public:
    int totalDocs;
    unordered_map<string, int> docFreq;
    unordered_map<string, unordered_set<int>> seenInDoc;
    Trie();
    void insertarpalabra(const string& palabra, int id, int pesoCampo) const;
    unordered_map<int,double> buscarNodo(const string& clave) const;
    unordered_map<int,double> buscarScores(const string& query) const;
};




#endif