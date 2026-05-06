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

struct Resultado {
    int id;
    double score;
};
struct Nodo {
    unordered_map<char, Nodo*> hijos;
    std::unordered_map<int, int >freq; //TF
    bool esFinDePalabra;
    Nodo() : esFinDePalabra(false) {}
};
class Trie {
    Nodo* raiz;

    unordered_map<string,int> docFreq; //DF
    int totalDocs = 0;

public:
    Trie();

    void insertarCompleto(const string& texto, int id, int pesoCampo) const;
    void construirIndice(const unordered_map<int, string>& data);

    vector<int> buscar(const string& query);
};
void dfsRecolectar(Nodo* nodo, unordered_map<int,double>& score, double idf,  bool esRaizBusqueda, int& contador);
vector<Resultado> ordenarResultados(unordered_map<int,double>& score);
#endif //PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
