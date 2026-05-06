#include "Trie.h"

Trie::Trie() : raiz(new Nodo()), totalDocs(0) {}


void Trie::limpiarNodo(Nodo* nodo) {
    if (!nodo) return;
    for (auto& par : nodo->hijos) {
        limpiarNodo(par.second);
    }
    delete nodo;
}
Trie::~Trie() {
    limpiarNodo(raiz);
}
void Trie::insertarpalabraYTrigramas(const string& palabra, int id, int pesoCampo) {
    int n = palabra.size();

    for (int i = 0; i < n; i++) {
        Nodo* nodo = raiz;

        for (int j = i; j < n; j++) {
            char c = palabra[j];

            if (!nodo->hijos.count(c))
                nodo->hijos[c] = new Nodo();

            nodo = nodo->hijos[c];
            nodo->freq[id] += pesoCampo;
        }
        nodo->esFinDePalabra = true;
    }
}
void Trie::insertarCompleto(const string& texto, int id, int pesoCampo) {
    stringstream ss(texto);
    string palabra;

    while (ss >> palabra) {
        insertarpalabraYTrigramas(palabra, id, pesoCampo);
        if (!seenInDoc[palabra].count(id)) {
            seenInDoc[palabra].insert(id);
            docFreq[palabra]++;   // solo 1 vez por documento
        }
    }
}

unordered_map<int,int> Trie::buscarNodo(const string& clave) const {
    const Nodo* nodo = raiz;

    for (char c : clave) {
        if (!nodo->hijos.count(c)) return {};
        nodo = nodo->hijos.at(c);
    }

    unordered_map<int,int> resultado;

    if (nodo->esFinDePalabra) {
        for (auto &[id, freq] : nodo->freq) {
            resultado[id] += freq*0.5; // boost fuerte
        }
    }
    else {
        for (auto &[id, freq] : nodo->freq) {
            resultado[id] += freq;
        }
    }

    return resultado;
}
vector<int> Trie::buscar(const string& query) const {
    unordered_map<int,double> score;
    unordered_map<int,int> matchCount;

    stringstream ss(query);
    string token;

    int totalTokens = 0;

    while (ss >> token) {

        if (token.size() <= 2) continue;
        totalTokens++;

        auto resultados = buscarNodo(token);

        double idf;

        auto it = docFreq.find(token);

        int df = (it != docFreq.end()) ? it->second : 0;

        idf = log((double)totalDocs / (1.0 + df));

        for (auto &[id, freq] : resultados) {

            double tf = (double)freq;

            double add = tf * idf;

            score[id] += add;

            matchCount[id]++;
        }
    }

    // penalización fuerte por incompletitud
    for (auto &[id, sc] : score) {
        if (matchCount[id] < totalTokens) {
            sc *= 0.2;  // más agresivo = menos falsos positivos
        }
    }

    vector<Resultado> orden;

    for (auto &[id, sc] : score) {
        orden.push_back({id, (int)sc});
    }

    sort(orden.begin(), orden.end(),
         [](const Resultado &a, const Resultado &b) {
             return a.score > b.score;
         });

    vector<int> res;

    for (int i = 0; i < 5 && i < (int)orden.size(); i++) {
        res.push_back(orden[i].id);
    }

    return res;
}