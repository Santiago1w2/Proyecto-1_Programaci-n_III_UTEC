#include "Trie.h"

//Constructor de la clase Trie
Trie::Trie() {
    raiz = new Nodo();
    totalDocs = 0;
}



//Insersión de palabras
void Trie::insertarpalabra(const string& palabra, const int id, const int pesoCampo) const {
    int n = palabra.size();
    if (n < 3) {
        return;
    }

    // 1. Insersión de la palabra completa
    Nodo* nodo = raiz;

    for(char c : palabra) {
        unsigned char uc = static_cast<unsigned char>(c);

        if(nodo->hijos[uc] == nullptr) {
            cout << "Creando nodo para: " << c << endl;
            nodo->hijos[uc] = new Nodo();
        }

        nodo = nodo->hijos[uc];
        auto it = nodo->freq.find(id);

        if(it == nodo->freq.end()) {
            nodo->freq[id] = pesoCampo * 2;
        }
        else {
            it->second += pesoCampo * 2;
        }
    }
    nodo->esPalabraExacta = true;

    // 2. Insersión de sufijos con un máximo de 6 caracteres
    for(int i = 1; i < n; i++) {
        Nodo* actual = raiz;
        for(int j = i;j < min(n, i + MAX_LONG);j++) {
            char c = palabra[j];
            unsigned char uc = static_cast<unsigned char>(c);

            if(actual->hijos[uc] == nullptr) {
                actual->hijos[uc] = new Nodo();
            }

            actual = actual->hijos[uc];
            auto it = actual->freq.find(id);

            if(it == actual->freq.end()) {
                actual->freq[id] = pesoCampo;
            }
            else {
                it->second += pesoCampo;
            }
        }
    }
}


unordered_map<int,double> Trie::buscarNodo(const string& clave) const {
    const Nodo* nodo = raiz;

    for(char c : clave) {
        unsigned char uc = static_cast<unsigned char>(c);

        if(nodo->hijos[uc] == nullptr) {
            return {};
        }
        nodo = nodo->hijos[uc];
    }

    unordered_map<int,double> resultado;
    double boost;
    if (nodo->esPalabraExacta) {
        boost = 1.5;
    }
    else {
        boost = 1.0;
    }

    for(const auto& [id, freq] : nodo->freq) {
        resultado[id] = freq * boost;
    }
    return resultado;
}



unordered_map<int, double> Trie::buscarScores(const string &query) const {
    unordered_map<int,double> score;
    unordered_map<int,int> matchCount;
    stringstream ss(query);
    string token;
    int totalTokens = 0;

    while (ss >> token) {
        token = aMinuscula(token);
        if (token.size() <= 2)
            continue;
        totalTokens++;
        auto resultados = buscarNodo(token);
        int df = 1;
        auto it = docFreq.find(token);
        if (it != docFreq.end())
            df = it->second;
        double idf =
            log(1.0 + ((double)totalDocs / (1.0 + df)));
        for (auto &[id, tf] : resultados) {
            score[id] += tf * idf;
            matchCount[id]++;
        }
    }

    // Penalización basada en cobertura
    for (auto &[id, sc] : score) {
        double cobertura =
            (double)matchCount[id] / totalTokens;
        sc *= (0.5 + 0.5 * cobertura);
    }
    return score;
}
