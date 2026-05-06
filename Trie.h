//
// Created by smora on 6/05/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <cctype>
using namespace std;

struct Nodo {
    unordered_map<char, Nodo*> hijos;
    unordered_map<int, double> freq; // docID -> peso acumulado (TF)
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
    unordered_map<string, int> docFreq; // DF para palabras y 3-gramas
    int totalDocs;

    // util
    static inline bool esValida(const string& w) { return w.size() >= 3; }



public:
    Trie();
    ~Trie();

    // Inserta texto completo (ya preprocesado: minúsculas, sin tildes, limpio)
    void insertarCompleto(const string& texto, int id, int pesoCampo);

    // Construye DF (palabras + 3-gramas)
    void construirIndice(const unordered_map<int, string>& data);

    // Búsqueda con ranking
    vector<int> buscar(const string& query) const;
    void insertarpalabraYTrigramas(const string& palabra, int id, int pesoCampo);
    unordered_map<int,double> buscarNodo(const string& clave) const;
};
#endif //PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
