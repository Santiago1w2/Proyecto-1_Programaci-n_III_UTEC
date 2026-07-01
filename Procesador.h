#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H

#include "Trie.h"
#include "Utilidades.h"
#include "LimPelis.h"
#include "RankingStrategy.h"

class Procesador {
    vector<unique_ptr<Trie>> tries;
    int totalDocsPr = 0;
    unordered_map<string, int> docFreqPr;
    unordered_map<int, DocumentoIndexado> documentosProcesados;
    unique_ptr<RankingStrategy> rankingStrategy;
public:
    Procesador();
    void procesar(const unordered_map<int, DataLimpia>& peliculas);
    vector<int> buscar(const string& consulta);
    void setRankingStrategy(unique_ptr<RankingStrategy> estrategia);
    void setDocFreq(unordered_map<string, int> _docFreq){docFreqPr = _docFreq;}
};

//Funciones de pre-procesamiento
vector<string> tokenizar(const string& texto);
void agregarTokens(DocumentoIndexado& doc,const string& texto,int peso);
DocumentoIndexado procesarMovie(int movieID,const DataLimpia& movie);
void cargarData(Procesador& preprocesador, unordered_map<int, Movie>& dataSucia, unordered_map<int, DataLimpia>& dataLimpia,  bool& datoslisto);
#endif //PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
