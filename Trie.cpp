#include "Trie.h"

Trie::Trie() {
    raiz = new Nodo();
    raizSufijos = new Nodo();
    totalDocs = 0;
    docFreqRef = &docFreq;
}

Trie::~Trie() {
    liberarNodo(raiz);
    liberarNodo(raizSufijos);
}

void Trie::liberarNodo(Nodo* nodo) const {
    if (nodo == nullptr) {
        return;
    }

    for (const auto& [_, hijo] : nodo->hijos) {
        liberarNodo(hijo);
    }
    delete nodo;
}

void Trie::insertarRuta(Nodo* inicio, const string& palabra, int id, int peso) const {
    Nodo* nodo = inicio;
    for (char c : palabra) {
        const unsigned char uc = static_cast<unsigned char>(c);
        auto hijo = nodo->hijos.find(uc);
        if (hijo == nodo->hijos.end()) {
            hijo = nodo->hijos.emplace(uc, new Nodo()).first;
        }
        nodo = hijo->second;
    }

    nodo->esPalabraExacta = true;
    auto it = nodo->freq.find(id);
    if (it == nodo->freq.end()) {
        nodo->freq[id] = peso;
    } else {
        it->second += peso;
    }
}

void Trie::insertarpalabra(const string& palabra, const int id, const int pesoCampo) const {
    const int n = static_cast<int>(palabra.size());
    if (n < 3) {
        return;
    }

    Nodo* nodo = raiz;
    for (char c : palabra) {
        const unsigned char uc = static_cast<unsigned char>(c);
        auto hijo = nodo->hijos.find(uc);
        if (hijo == nodo->hijos.end()) {
            hijo = nodo->hijos.emplace(uc, new Nodo()).first;
        }

        nodo = hijo->second;
        auto it = nodo->freq.find(id);
        if (it == nodo->freq.end()) {
            nodo->freq[id] = pesoCampo * 2;
        } else {
            it->second += pesoCampo * 2;
        }
    }
    nodo->esPalabraExacta = true;

    string reversa = palabra;
    reverse(reversa.begin(), reversa.end());
    insertarRuta(raizSufijos, reversa, id, pesoCampo);

    // Inserta sufijos/subcadenas limitadas. Los nodos de longitud 1 y 2 se
    // crean para conectar rutas, pero no guardan postings porque no se buscan.
    for (int i = 1; i < n; i++) {
        Nodo* actual = raiz;
        for (int j = i; j < min(n, i + MAX_LONG); j++) {
            const unsigned char uc = static_cast<unsigned char>(palabra[j]);
            auto hijo = actual->hijos.find(uc);
            if (hijo == actual->hijos.end()) {
                hijo = actual->hijos.emplace(uc, new Nodo()).first;
            }

            actual = hijo->second;
            if (j - i + 1 < 3) {
                continue;
            }

            auto it = actual->freq.find(id);
            if (it == actual->freq.end()) {
                actual->freq[id] = pesoCampo;
            } else {
                it->second += pesoCampo;
            }
        }
    }
}

unordered_map<int, double> Trie::buscarDesde(const Nodo* inicio, const string& clave) const {
    const Nodo* nodo = inicio;

    for (char c : clave) {
        const unsigned char uc = static_cast<unsigned char>(c);
        auto hijo = nodo->hijos.find(uc);
        if (hijo == nodo->hijos.end()) {
            return {};
        }
        nodo = hijo->second;
    }

    unordered_map<int, double> resultado;
    resultado.reserve(nodo->freq.size());
    const double boost = nodo->esPalabraExacta ? 1.5 : 1.0;
    for (const auto& [id, freq] : nodo->freq) {
        resultado[id] = freq * boost;
    }
    return resultado;
}

unordered_map<int, double> Trie::buscarNodo(const string& clave) const {
    return buscarDesde(raiz, clave);
}

unordered_map<int, double> Trie::buscarSufijoLargo(const string& clave) const {
    string reversa = clave;
    reverse(reversa.begin(), reversa.end());
    return buscarDesde(raizSufijos, reversa);
}

unordered_map<int, double> Trie::buscarScores(const string& query) const {
    unordered_map<int, double> score;
    unordered_map<int, int> matchCount;
    stringstream ss(query);
    string token;
    int totalTokens = 0;
    const auto* frecuencias = docFreqRef != nullptr ? docFreqRef : &docFreq;

    while (ss >> token) {
        token = aMinuscula(token);
        if (token.size() <= 2) {
            continue;
        }

        totalTokens++;
        auto resultados = buscarNodo(token);
        if (resultados.empty() && token.size() > MAX_LONG) {
            resultados = buscarSufijoLargo(token);
        }
        int df = 1;
        auto it = frecuencias->find(token);
        if (it != frecuencias->end()) {
            df = it->second;
        }

        const double idf = log(1.0 + (static_cast<double>(totalDocs) / (1.0 + df)));
        for (auto& [id, tf] : resultados) {
            score[id] += tf * idf;
            matchCount[id]++;
        }
    }

    if (totalTokens == 0) {
        return {};
    }

    for (auto& [id, sc] : score) {
        const double cobertura = static_cast<double>(matchCount[id]) / totalTokens;
        sc *= (0.5 + 0.5 * cobertura);
    }
    return score;
}
