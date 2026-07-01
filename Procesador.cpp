#include "Procesador.h"


//Funcio para tokenizar un string (separarlas palabas por cada espacio en el string)
vector<string> tokenizar(const string &texto) {
    vector<string> tokens;
    stringstream ss(texto);
    string palabra;
    while (ss >> palabra) {
        tokens.push_back(palabra);
    }
    return tokens;
}

void agregarTokens(DocumentoIndexado &doc, const string &texto, int peso) {
    vector<string> tokens =tokenizar(texto);
    for (const string& t : tokens)
        doc.tokens.push_back({t,peso});
}

DocumentoIndexado procesarMovie(int movieID, const DataLimpia &movie) {
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


//Constructor para definir la cantidad threads que se usaran y cantidad tries que se crearán
Procesador::Procesador() : Procesador(NUM_THREADS) {}

Procesador::Procesador(int cantidadTries) {
    if (cantidadTries <= 0) {
        cantidadTries = 1;
    }
    for(int i = 0; i < cantidadTries; i++){
        tries.push_back(make_unique<Trie>());
    }
}

void Procesador::procesarSecuencial(const unordered_map<int, DataLimpia>& peliculas) {
    totalDocsPr = peliculas.size();
    docFreqPr.clear();
    if (tries.empty()) {
        tries.push_back(make_unique<Trie>());
    }

    Trie& trie = *tries[0];
    unordered_map<string, int> docFreqLocal;
    for(const auto& [movieID, movie] : peliculas) {
        DocumentoIndexado doc = procesarMovie(movieID, movie);
        unordered_set<string> vistos;

        for(const TokenInfo& tk : doc.tokens) {
            string tokenMinuscula = aMinuscula(tk.token);
            trie.insertarpalabra(tokenMinuscula, movieID, tk.peso);

            if(!vistos.count(tokenMinuscula)) {
                docFreqLocal[tokenMinuscula]++;
                vistos.insert(tokenMinuscula);
            }
        }
    }

    docFreqPr = move(docFreqLocal);
    trie.setDocFreq(docFreqPr);
    trie.setTotalDocs(totalDocsPr);
}


void Procesador::procesar(const unordered_map<int, DataLimpia>& peliculas){
    totalDocsPr = peliculas.size();
    vector<pair<int, DataLimpia>> datos;
    for(const auto& p : peliculas){
        datos.push_back(p);
    }
    int cantidadTries = static_cast<int>(tries.size());
    if (cantidadTries <= 0) {
        tries.push_back(make_unique<Trie>());
        cantidadTries = 1;
    }
    vector<thread> threads;
    vector<unordered_map<string,int>> docFreqLocales(cantidadTries);

    auto worker = [this,&datos,&docFreqLocales]
    (
        int inicio,
        int fin,
        int indiceTrie
        )
    {
        Trie& trie = *tries[indiceTrie];
        auto& localFreq = docFreqLocales[indiceTrie];

        for(int i = inicio; i < fin; i++) {
            int movieID = datos[i].first;
            const DataLimpia& movie = datos[i].second;
            DocumentoIndexado doc = procesarMovie(movieID, movie);
            unordered_set<string> vistos;

            for(const TokenInfo& tk : doc.tokens) {
                string tokenMinuscula = aMinuscula(tk.token);
                trie.insertarpalabra(tokenMinuscula, movieID, tk.peso);

                if(!vistos.count(tokenMinuscula)) {
                    localFreq[tokenMinuscula]++;
                    vistos.insert(tokenMinuscula);
                }
            }
        }
    };

    int bloque = datos.empty() ? 0 : static_cast<int>(datos.size()) / cantidadTries;
    int fin;
    for(int t = 0;t < cantidadTries;t++) {
        int inicio = t * bloque;

        if (t==cantidadTries-1)
            fin = static_cast<int>(datos.size());
        else {
            fin = inicio + bloque;
        }
        threads.emplace_back(worker, inicio, fin, t);
    }

    for(auto& t : threads){t.join();}

    unordered_map<string,int> globalDocFreq;

    for(const auto& mapa :docFreqLocales) {
        for(const auto& [token,freq]: mapa) {
            globalDocFreq[token]+= freq;
        }
    }

    docFreqPr = move(globalDocFreq);

    for(auto& trie : tries) {
        trie->setDocFreq(docFreqPr);
        trie->setTotalDocs(totalDocsPr);
    }
}


vector<int> Procesador::buscar(const string& consulta) {
    vector<unordered_map<int,double>> resultados(tries.size());
    vector<thread> threads;

    for(size_t i = 0;i < tries.size();i++) {
        threads.emplace_back([&,i]()
            {
                resultados[i] = tries[i]->buscarScores(consulta);
            }
        );
    }

    for(auto& t : threads){t.join();}

    unordered_map<int,double>scoreGlobal;

    for(const auto& parcial : resultados) {
        for(const auto& [id,score]: parcial) {
            scoreGlobal[id]+= score;
        }
    }

    vector<Resultado> orden;
    for(const auto& [id,score]: scoreGlobal) {
        orden.push_back({id,score});
    }

    sort(orden.begin(),orden.end(),[](const Resultado& a,const Resultado& b)
        {return a.score > b.score;});

    vector<int> respuesta;
    for(size_t i = 0;i < 5 && i < orden.size();i++) {
        respuesta.push_back(orden[i].id);
    }

    return respuesta;
}
