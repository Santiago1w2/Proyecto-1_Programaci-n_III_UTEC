//
// Created by smora on 6/05/2026.
//

#include "Trie.h"

Trie::Trie() : raiz(new Nodo()), totalDocs(0) {}

Trie::~Trie() {
    delete raiz;
}

// Inserta un palabra completo
void Trie::insertarpalabraYTrigramas(const string& palabra, int id, int pesoCampo) {
    if (!esValida(palabra)) return;

    // caso uno insertar token completo (prefijos)
    Nodo* nodo = raiz;
    for (char c : palabra) {
        if (!nodo->hijos.count(c)) nodo->hijos[c] = new Nodo();
        nodo = nodo->hijos[c];
        nodo->freq[id] += pesoCampo; // TF acumulado con peso de campo
    }
    nodo->esFinDePalabra = true;

    //caso 2 3-gramas subs string de tamaños 3
    int n = (int)palabra.size();
    for (int i = 0; i <= n - 3; ++i) {
        Nodo* nodoSub = raiz;
        for (int j = 0; j < 3; ++j) {
            char c = palabra[i + j];
            if (!nodoSub->hijos.count(c)) nodoSub->hijos[c] = new Nodo();
            nodoSub = nodoSub->hijos[c];
            // menor peso para substrings
            nodoSub->freq[id] += max(1, pesoCampo / 2);
        }
    }
}

// Inserta texto completo (tokeniza por espacios)
void Trie::insertarCompleto(const string& texto, int id, int pesoCampo) {
    stringstream ss(texto);
    string palabra;
    while (ss >> palabra) {
        insertarpalabraYTrigramas(palabra, id, pesoCampo);
    }
}

// Construye DF para palabras y 3-gramas (sin duplicar dentro del mismo doc)
void Trie::construirIndice(const unordered_map<int, string>& data) {
    totalDocs = (int)data.size();

    for (const auto& [id, texto] : data) {
        unordered_set<string> vistas; // evitar contar repetidos en el mismo doc
        stringstream ss(texto);
        string token;

        while (ss >> token) {
            if (!esValida(token)) continue;

            // DF de palabra completa
            if (vistas.insert(token).second) {
                docFreq[token]++;
            }

            // DF de 3-gramas
            int n = (int)token.size();
            for (int i = 0; i <= n - 3; ++i) {
                string sub = token.substr(i, 3);
                if (vistas.insert(sub).second) {
                    docFreq[sub]++;
                }
            }
        }
    }
}

// Busca un nodo exacto en el Trie
unordered_map<int,double> Trie::buscarNodo(const string& clave) const {
    const Nodo* nodo = raiz;

    for (char c : clave) {
        if (!nodo->hijos.count(c)) return {};
        nodo = nodo->hijos.at(c);
    }

    return nodo->freq; // copia
}

// Búsqueda con TF-IDF + log(tf) + bonus por coincidencia completa
vector<int> Trie::buscar(const string& query) const {
    unordered_map<int,double> score;
    unordered_map<int,int> coincidencias;

    stringstream ss(query);
    string token;
    int totalPartes = 0;

    while (ss >> token) {
        if (!esValida(token)) continue;

        vector<string> partes;

        // Si es >= 3, usar 3-gramas; si no, usar el token directo
        if ((int)token.size() >= 3) {
            for (int i = 0; i <= (int)token.size() - 3; ++i) {
                partes.push_back(token.substr(i, 3));
            }
        } else {
            partes.push_back(token);
        }

        for (const string& p : partes) {
            totalPartes++;

            auto resultados = buscarNodo(p);

            // IDF
            int df = docFreq.count(p) ? docFreq.at(p) : totalDocs;
            double idf = log((double)totalDocs / (1.0 + df));

            for (const auto& [id, tf] : resultados) {
                // TF logarítmico
                double wtf = 1.0 + log(tf);
                score[id] += wtf * idf;
                coincidencias[id]++;
            }
        }
    }

    if (totalPartes == 0) return {};

    // Bonus por cubrir todas las partes de la query
    for (auto& [id, c] : coincidencias) {
        if (c == totalPartes) {
            score[id] *= 1.5;
        }
        // Normalización por longitud de query
        score[id] /= totalPartes;
    }

    // Ordenar
    vector<Resultado> orden;
    orden.reserve(score.size());
    for (const auto& [id, sc] : score) {
        orden.push_back({id, sc});
    }

    sort(orden.begin(), orden.end(),
         [](const Resultado& a, const Resultado& b) {
             return a.score > b.score;
         });

    // Top-K
    vector<int> res;
    for (int i = 0; i < 5 && i < (int)orden.size(); ++i) {
        res.push_back(orden[i].id);
    }

    return res;
}