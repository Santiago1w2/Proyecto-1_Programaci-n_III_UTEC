//
// Created by burgo on 9/6/2026.
//

#include "PreProcesador.h"


vector<string> Preprocesador::tokenizar(const string &texto) {
    vector<string> tokens;
    stringstream ss(texto);
    string palabra;
    while (ss >> palabra) {
        tokens.push_back(palabra);
    }
    return tokens;
}

void Preprocesador::agregarTokens(DocumentoIndexado &doc, const string &texto, int peso) {
    vector<string> tokens =tokenizar(texto);
    for (const string& t : tokens)
        doc.tokens.push_back({t,peso});
}

DocumentoIndexado Preprocesador::procesarMovie(int movieID, const DataLimpia &movie) {
    DocumentoIndexado doc;
    doc.movieID = movieID;

    agregarTokens(doc,movie.getTitle(),5);
    agregarTokens(doc,movie.getRelease_year(),1);
    agregarTokens(doc,movie.getOrigin(),1);
    agregarTokens(doc,movie.getDirector(),1);
    agregarTokens(doc,movie.getCast(),1);
    agregarTokens(doc,movie.getGenre(),1);
    agregarTokens(doc,movie.getPlot(),2);
    return doc;
}


void Preprocesador::preprocesar(const unordered_map<int, DataLimpia> &peliculas)
{
    totalDocs = peliculas.size();
    for (const auto& par : peliculas)
    {
        int movieID = par.first;
        const DataLimpia& movie = par.second;
        DocumentoIndexado doc =
            procesarMovie(movieID,movie);
        documentosProcesados[movieID] = doc;
        unordered_set<string> vistos;
        for (const TokenInfo& tk : doc.tokens)
        {  
            // INSERTAR EN EL TRIE
            trie.insertarpalabra(
                tk.token,
                movieID,
                tk.peso
            );
            // DOC FREQ
            if (!vistos.count(tk.token))
            {
                docFreq[tk.token]++;
                vistos.insert(tk.token);
            }
        }
    }
    trie.totalDocs = totalDocs;
    trie.docFreq = docFreq;
}




vector<int> Preprocesador::buscar(const string& consulta){
    return trie.buscar(consulta);
}