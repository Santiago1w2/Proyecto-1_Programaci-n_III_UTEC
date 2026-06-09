#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H

#include "CClases.h"
using namespace std;


struct Nodo {
    unordered_map<char, Nodo*> hijos;
    unordered_map<int, int> freq; // docID -> conteo simple
    bool esPalabraExacta= false;
    ~Nodo() {
        for (auto &p : hijos) delete p.second;
    }
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
    void insertarCompleto(const string& texto, int id, int pesoCampo);
    void insertarpalabra(const string& palabra, int id, int pesoCampo) const;
    vector<int> buscar(const string& query) const;
    unordered_map<int,double> buscarNodo(const string& clave) const;

};

#endif