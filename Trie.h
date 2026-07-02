#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_TRIE_H

#include "Utilidades.h"

struct Nodo {
    unordered_map<unsigned char, Nodo*> hijos;
    unordered_map<int, int> freq;
    bool esPalabraExacta= false;
};


struct Resultado {
    int id;
    double score;
};


class Trie {
    Nodo* raiz;
    Nodo* raizSufijos;
    int totalDocs;
    unordered_map<string, int> docFreq;
    const unordered_map<string, int>* docFreqRef;
    void liberarNodo(Nodo* nodo) const;
    void insertarRuta(Nodo* inicio, const string& palabra, int id, int peso) const;
    unordered_map<int,double> buscarDesde(const Nodo* inicio, const string& clave) const;
    unordered_map<int,double> buscarSufijoLargo(const string& clave) const;
public:
    Trie();
    ~Trie();
    void insertarpalabra(const string& palabra, int id, int pesoCampo) const;
    unordered_map<int,double> buscarNodo(const string& clave) const;
    unordered_map<int,double> buscarScores(const string& query) const;
    void setDocFreq(unordered_map<string, int> _docFreq){docFreq = move(_docFreq); docFreqRef = &docFreq;}
    void setDocFreqReferencia(const unordered_map<string, int>* _docFreq){docFreqRef = _docFreq;}
    void setTotalDocs(int _totalDocs){totalDocs = _totalDocs;}
};




#endif
