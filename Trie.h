#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H

#include "Utilidades.h"

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
    int totalDocs;
    unordered_map<string, int> docFreq;
    void liberar(Nodo* nodo) const;
public:
    Trie();
    ~Trie();
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    void insertarpalabra(const string& palabra, int id, int pesoCampo) const;
    unordered_map<int,double> buscarNodo(const string& clave) const;
    unordered_map<int,double> buscarScores(const string& query) const;
    void setDocFreq(unordered_map<string, int> _docFreq){docFreq = _docFreq;}
    void setTotalDocs(int _totalDocs){totalDocs = _totalDocs;}
};




#endif
