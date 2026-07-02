#include "Procesador.h"

namespace {
constexpr int MAX_TOKENS_PARA_FRASES = 40;
constexpr int MAX_PALABRAS_FRASE = 3;

string unirTokens(const vector<string>& tokens, size_t inicio, size_t cantidad) {
    string frase = tokens[inicio];
    for (size_t i = 1; i < cantidad; ++i) {
        frase += " " + tokens[inicio + i];
    }
    return frase;
}

vector<string> tokensValidosMinuscula(const string& texto) {
    vector<string> tokens = tokenizar(texto);
    vector<string> validos;
    validos.reserve(tokens.size());
    for (string token : tokens) {
        token = aMinuscula(token);
        if (token.size() > 2) {
            validos.push_back(token);
        }
    }
    return validos;
}

void registrarMetricaTiempo(const string& operacion, long long tiempoMs, size_t cantidad = 0) {
    ofstream archivo("metricas_tiempo.txt", ios::app);
    if (!archivo.is_open()) {
        return;
    }

    archivo << operacion << " | tiempo_ms: " << tiempoMs;
    if (cantidad > 0) {
        archivo << " | cantidad: " << cantidad;
    }
    archivo << endl;
}
}

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
    vector<string> tokens = tokenizar(texto);
    for (const string& t : tokens) {
        doc.tokens.push_back({t,peso});
    }

    if (tokens.size() > MAX_TOKENS_PARA_FRASES) {
        return;
    }

    for (string& token : tokens) {
        token = aMinuscula(token);
    }

    for (size_t cantidad = 2; cantidad <= MAX_PALABRAS_FRASE; ++cantidad) {
        if (tokens.size() < cantidad) {
            break;
        }
        for (size_t i = 0; i + cantidad <= tokens.size(); ++i) {
            bool fraseValida = true;
            for (size_t j = 0; j < cantidad; ++j) {
                if (tokens[i + j].size() <= 2) {
                    fraseValida = false;
                    break;
                }
            }
            if (fraseValida) {
                doc.tokens.push_back({unirTokens(tokens, i, cantidad), peso * 2});
            }
        }
    }
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
Procesador::Procesador() : rankingStrategy(make_unique<RankingPorScoreStrategy>()) {
    for(int i = 0; i < NUM_THREADS; i++){
        tries.push_back(make_unique<Trie>());
    }
}

Procesador::~Procesador() {
    vector<thread> destructores;
    destructores.reserve(tries.size());

    for (auto& trie : tries) {
        destructores.emplace_back([trie = move(trie)]() mutable {
            trie.reset();
        });
    }

    for (auto& t : destructores) {
        if (t.joinable()) {
            t.join();
        }
    }

    tries.clear();
}


void Procesador::procesar(const unordered_map<int, DataLimpia>& peliculas){
    auto inicioTiempo = chrono::high_resolution_clock::now();
    totalDocsPr = peliculas.size();
    vector<pair<int, const DataLimpia*>> datos;
    datos.reserve(peliculas.size());
    for(const auto& p : peliculas){
        datos.push_back({p.first, &p.second});
    }
    vector<thread> threads;
    vector<unordered_map<string,int>> docFreqLocales(NUM_THREADS);

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
            const DataLimpia& movie = *datos[i].second;
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

    int bloque = datos.size() / NUM_THREADS;
    int fin;
    for(int t = 0;t < NUM_THREADS;t++) {
        int inicio = t * bloque;

        if (t==NUM_THREADS-1)
            fin = datos.size();
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
        trie->setDocFreqReferencia(&docFreqPr);
        trie->setTotalDocs(totalDocsPr);
    }

    auto finTiempo = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(finTiempo - inicioTiempo);
    registrarMetricaTiempo("INSERCION_ARBOL_PARALELA", duracion.count(), peliculas.size());
}


vector<int> Procesador::buscar(const string& consulta) {
    auto inicioTiempo = chrono::high_resolution_clock::now();
    vector<unordered_map<int,double>> resultados(tries.size());
    vector<thread> threads;

    for(int i = 0;i < tries.size();i++) {
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

    vector<string> tokensConsulta = tokensValidosMinuscula(consulta);
    if (tokensConsulta.size() > 1 && tokensConsulta.size() <= MAX_PALABRAS_FRASE) {
        string frase = unirTokens(tokensConsulta, 0, tokensConsulta.size());
        int df = 1;
        auto it = docFreqPr.find(frase);
        if (it != docFreqPr.end()) {
            df = it->second;
        }
        double idf = log(1.0 + (static_cast<double>(totalDocsPr) / (1.0 + df)));
        for (auto& trie : tries) {
            auto coincidenciasFrase = trie->buscarNodo(frase);
            for (const auto& [id, tf] : coincidenciasFrase) {
                scoreGlobal[id] += tf * idf * 2.0;
            }
        }
    }

    vector<int> respuesta = rankingStrategy->ordenar(scoreGlobal);
    auto finTiempo = chrono::high_resolution_clock::now();
    auto duracion = chrono::duration_cast<chrono::milliseconds>(finTiempo - inicioTiempo);
    registrarMetricaTiempo("BUSQUEDA_ARBOL_PARALELA", duracion.count(), respuesta.size());
    return respuesta;
}

void Procesador::setRankingStrategy(unique_ptr<RankingStrategy> estrategia) {
    if (estrategia) {
        rankingStrategy = move(estrategia);
    }
}
void cargarData(Procesador& preprocesador, unordered_map<int, Movie>& dataSucia, unordered_map<int, DataLimpia>& dataLimpia, bool& datoslisto) {
    dataSucia = leerPeliculas("peliculas.csv");
    exportarDataLimpiaCSV(dataSucia, "datosLimpios.csv", dataLimpia);
    auto inicio = std::chrono::high_resolution_clock::now();
    preprocesador.procesar(dataLimpia);
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion =std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    std::ofstream archivo("metricas_tiempo.txt", std::ios::app);

    if (archivo.is_open()) {
        archivo << "INSERCION_ARBOL | tiempo_ms: "
                << duracion.count()
                << " ms" << std::endl;
        archivo.close();
    }
    datoslisto = true;
}
