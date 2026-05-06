#include "Trie.h"

Trie::Trie() : raiz(new Nodo()), totalDocs(0) {}

Trie::~Trie() {
    delete raiz;
}
void Trie::insertarpalabraYTrigramas(const string& palabra, int id, int pesoCampo) {
    if (!esValida(palabra)) return;

    // palabra completa
    Nodo* nodo = raiz;
    for (char c : palabra) {
        if (!nodo->hijos.count(c))
            nodo->hijos[c] = new Nodo();
        nodo = nodo->hijos[c];
    }
    nodo->esFinDePalabra = true;
    nodo->freq[id] += pesoCampo;

    // 3-gramas (controlado, NO explosión)
    int n = palabra.size();
    for (int i = 0; i <= n - 3; i++) {
        Nodo* aux = raiz;

        for (int j = 0; j < 3; j++) {
            char c = palabra[i + j];

            if (!aux->hijos.count(c))
                aux->hijos[c] = new Nodo();

            aux = aux->hijos[c];
        }

        aux->freq[id] += 1;
    }
}
void Trie::insertarCompleto(const string& texto, int id, int pesoCampo) {
    stringstream ss(texto);
    string palabra;

    while (ss >> palabra) {
        insertarpalabraYTrigramas(palabra, id, pesoCampo);
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
            resultado[id] += freq * 3; // boost fuerte
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
    unordered_map<int,int> score;
    unordered_map<int,int> matchCount;

    stringstream ss(query);
    string token;

    int totalTokens = 0;

    while (ss >> token) {
        if (token.size() == 1) continue;

        totalTokens++;

        auto resultados = buscarNodo(token);

        for (auto &[id, freq] : resultados) {
            score[id] += freq;

            matchCount[id]++;
        }

        if (resultados.empty()) {
            for (int i = 0; i <= (int)token.size() - 3; i++) {
                string sub = token.substr(i, 3);

                auto r = buscarNodo(sub);

                for (auto &[id, freq] : r) {
                    score[id] += 1;
                    matchCount[id]++;
                }
            }
        }
    }

    for (auto &[id, sc] : score) {
        if (matchCount[id] < totalTokens) {
            sc *= 0.4;  // baja fuerte si no coincide todo
        }
    }

    vector<Resultado> orden;

    for (auto &[id, sc] : score) {
        orden.push_back({id, sc});
    }

    sort(orden.begin(), orden.end(),
         [](const Resultado &a, const Resultado &b) {
             return a.score > b.score;
         });

    // top 5
    vector<int> res;

    for (int i = 0; i < 5 && i < (int)orden.size(); i++) {
        res.push_back(orden[i].id);
    }

    return res;
}