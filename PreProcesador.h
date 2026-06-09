//
// Created by burgo on 9/6/2026.
//

#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
#include "Trie.h"
#include "CClases.h"


class Preprocesador {
    Trie trie;
public:
    int totalDocs = 0;
    unordered_map<int, DocumentoIndexado> documentosProcesados;
    unordered_map<string, int> docFreq;

    vector<string> tokenizar(const string& texto);
    void agregarTokens(DocumentoIndexado& doc,const string& texto,int peso);
    DocumentoIndexado procesarMovie(int movieID,const DataLimpia& movie);
    void preprocesar(const unordered_map<int, DataLimpia>& peliculas);
    vector<int> buscar(const string& consulta);
};


#endif //PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
