#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>

using namespace std;

struct Nodo {
    unordered_map<char, Nodo*> hijos;
    unordered_map<int, int> freq; // docID -> conteo simple
    bool esFinDePalabra = false;

    ~Nodo() {
        for (auto &p : hijos) delete p.second;
    }
};

struct Resultado {
    int id;
    double score;
};

class Trie {
private:
    Nodo* raiz;
    int totalDocs;
    unordered_map<string, unordered_set<int>> seenInDoc;
    unordered_map<string, int> docFreq;

    static inline bool esValida(const string& w) {
        return !w.empty();
    }

public:
    Trie();
    ~Trie();

    static void limpiarNodo(Nodo* nodo);

    void insertarCompleto(const string& texto, int id, int pesoCampo);
void insertarpalabra(const string& palabra, const int id, const int pesoCampo) const;
    vector<int> buscar(const string& query) const;
    unordered_map<int,int> buscarNodo(const string& clave) const;
};

#endif