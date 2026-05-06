//
// Created by smora on 6/05/2026.
//

#include "Trie.h"
void dfsRecolectar(Nodo* nodo, unordered_map<int,double>& score, double idf,  bool esRaizBusqueda, int& contador) {

    if (contador > 100) return; //contador
    if (nodo->esFinDePalabra) {
        for (auto& par : nodo->freq) {
            int id = par.first;
            int tf = par.second;

            double factor = esRaizBusqueda ? 1.0 : 0.6;

            score[id] += tf * idf * factor;
        }
        contador++;
    }

    for (auto& hijo : nodo->hijos) {
        dfsRecolectar(hijo.second, score, idf, false, contador);
    }
}
vector<Resultado> ordenarResultados(unordered_map<int,double>& score) {

    vector<Resultado> lista;

    for (auto& par : score) {
        Resultado r;
        r.id = par.first;
        r.score = par.second;
        lista.push_back(r);
    }

    sort(lista.begin(), lista.end(),
         [](Resultado a, Resultado b) {
             return a.score > b.score;
         });

    return lista;
}
void Trie::insertarCompleto(const string& texto, int id, int pesoCampo) const {
    stringstream ss(texto);
    string palabra;

    while (ss >> palabra) {
        int n = palabra.size();
        if (n < 2) continue;

        // palabra compleja y prefijos
        {
            Nodo* nodo = raiz;
            for (char c : palabra) {
                if (!nodo->hijos[c]) nodo->hijos[c] = new Nodo();
                nodo = nodo->hijos[c];
            }
            nodo->esFinDePalabra = true;
            nodo->freq[id] += pesoCampo;
        }

        //sufijos y substrings
        for (int i = 1; i < n; i++) {
            int len = n - i;
            if (len < 3) continue;

            Nodo* nodo = raiz;
            for (int j = i; j < n; j++) {
                char c = palabra[j];
                if (!nodo->hijos[c]) nodo->hijos[c] = new Nodo();
                nodo = nodo->hijos[c];
            }

            nodo->esFinDePalabra = true;

            //esvitar el peso 0
            nodo->freq[id] += max(1, pesoCampo / 2);
        }
    }
}

void Trie::construirIndice(const unordered_map<int, string>& data) {
    totalDocs = data.size();

    for (auto& [id, texto] : data) {
        stringstream ss(texto);
        string palabra;

        unordered_set<string> vistas;

        while (ss >> palabra) {
            if (palabra.size() < 3) continue;

            if (vistas.insert(palabra).second) {
                docFreq[palabra]++;
            }
        }
    }
}
vector<int> Trie::buscar(const string& query) {

    unordered_map<int,double> score;

    stringstream ss(query);
    string palabra;

    while (ss >> palabra) {

        Nodo* nodo = raiz;
        for (char c : palabra) {
            if (!nodo->hijos.count(c)) {
                nodo = nullptr;
                break;
            }
            nodo = nodo->hijos[c];
        }

        if (!nodo) continue;

        // DF
        int df;

        if (docFreq.count(palabra)) {
            df = docFreq[palabra];
        } else {
            df = totalDocs;
        }

        double idf = log((double)totalDocs / (1 + df));
        int contador = 0;
        dfsRecolectar(nodo, score, idf, true, contador);}

    vector<Resultado> orden = ordenarResultados(score);

    // top 5
    vector<int> res;

    for (int i = 0; i < 5 && i < orden.size(); i++) {
        res.push_back(orden[i].id);
    }

    return res;
}