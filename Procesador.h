#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H

#include "Trie.h"
#include "Utilidades.h"

class Procesador {
    vector<unique_ptr<Trie>> tries;
    int totalDocsPr = 0;
    unordered_map<string, int> docFreqPr;
    unordered_map<int, DocumentoIndexado> documentosProcesados;
public:
    Procesador();
    explicit Procesador(int cantidadTries);
    void procesar(const unordered_map<int, DataLimpia>& peliculas);
    void procesarSecuencial(const unordered_map<int, DataLimpia>& peliculas);
    vector<int> buscar(const string& consulta);
    void setDocFreq(unordered_map<string, int> _docFreq){docFreqPr = _docFreq;}
};

//Funciones de pre-procesamiento
vector<string> tokenizar(const string& texto);
void agregarTokens(DocumentoIndexado& doc,const string& texto,int peso);
DocumentoIndexado procesarMovie(int movieID,const DataLimpia& movie);

#endif //PROYECTO_1_PROGRAMACION_III_UTEC_PREPROCESADOR_H
