#include "Trie.h"

Trie::Trie() : raiz(new Nodo()), totalDocs(0) {}


//Insersion de palabras
void Trie::insertarpalabra(
    const string& palabra,
    const int id,
    const int pesoCampo
) const
{
    int n = palabra.size();
    const int MAX_LEN = 6;

    // 1. Palabra completa
    Nodo* nodo = raiz;

    for(char c : palabra)
    {
        unsigned char uc =
            static_cast<unsigned char>(c);

        if(nodo->hijos[uc] == nullptr)
        {
            nodo->hijos[uc] =
                new Nodo();
        }

        nodo = nodo->hijos[uc];

        auto it = nodo->freq.find(id);

        if(it == nodo->freq.end())
        {
            nodo->freq[id] = pesoCampo * 2;
        }
        else
        {
            it->second += pesoCampo * 2;
        }
    }

    nodo->esPalabraExacta = true;

    // 2. Sufijos limitados a 6
    for(int i = 1; i < n; i++)
    {
        Nodo* actual = raiz;
        for(int j = i;j < min(n, i + MAX_LEN);j++){
            char c = palabra[j];
            unsigned char uc = static_cast<unsigned char>(c);

            if(actual->hijos[uc] == nullptr)
            {
                actual->hijos[uc] =
                    new Nodo();
            }

            actual = actual->hijos[uc];

            auto it = actual->freq.find(id);

            if(it == actual->freq.end())
            {
                actual->freq[id] = pesoCampo;
            }
            else
            {
                it->second += pesoCampo;
            }
        }
    }
}

void Trie::insertarCompleto(const string& texto,int id,int pesoCampo) {
    stringstream ss(texto);
    string palabra;
    while (ss >> palabra) {
        insertarpalabra(palabra, id, pesoCampo);
        auto& s = seenInDoc[palabra];
        if (s.find(id) == s.end())
        {
            s.insert(id);
            docFreq[palabra]++;
        }
    }
}

unordered_map<int,double>
Trie::buscarNodo(const string& clave) const
{
    const Nodo* nodo = raiz;

    for(char c : clave)
    {
        unsigned char uc =
            static_cast<unsigned char>(c);

        if(nodo->hijos[uc] == nullptr)
        {
            return {};
        }

        nodo = nodo->hijos[uc];
    }

    unordered_map<int,double> resultado;

    double boost =
        nodo->esPalabraExacta ? 1.5 : 1.0;

    for(const auto& [id, freq] : nodo->freq)
    {
        resultado[id] =
            freq * boost;
    }

    return resultado;
}



string aMinuscula(string texto)
{
    for(char& c : texto)
    {
        c = static_cast<char>(
                tolower(
                    static_cast<unsigned char>(c)
                )
            );
    }

    return texto;
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

    vector<Resultado> orden;
    for (auto &[id, sc] : score) {
        orden.push_back({id, sc});
    }

    sort(
        orden.begin(),
        orden.end(),
        [](const Resultado& a,
           const Resultado& b) {
            return a.score > b.score;
        }
    );

    vector<int> res;
    for (int i = 0;
         i < 5 && i < (int)orden.size();
         i++) {
        res.push_back(orden[i].id);
         }
    return score;
}
