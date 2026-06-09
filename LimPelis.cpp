#include "LimPelis.h"

// Acá decidí usar un unordered_map<string,string> porque algunos caracteres UTF-8 ocupan más de un byte y además algunos símbolos se convierten en más de una letra.
unordered_map<string, string> accents = {
    // A
    {"á","a"},{"à","a"},{"ä","a"},{"â","a"},{"ã","a"},{"å","a"},
    {"Á","a"},{"À","a"},{"Ä","a"},{"Â","a"},{"Ã","a"},{"Å","a"},
    {"ā","a"},{"Ā","a"},{"ă","a"},{"ǎ","a"},
    // E
    {"é","e"},{"è","e"},{"ë","e"},{"ê","e"},
    {"É","e"},{"È","e"},{"Ë","e"},{"Ê","e"},
    {"ē","e"},{"Ē","e"},{"ė","e"},{"ę","e"},{"ě","e"},
    // I
    {"í","i"},{"ì","i"},{"ï","i"},{"î","i"},
    {"Í","i"},{"Ì","i"},{"Ï","i"},{"Î","i"},
    {"ī","i"},{"Ī","i"},{"İ","i"},{"ı","i"},
    // O
    {"ó","o"},{"ò","o"},{"ö","o"},{"ô","o"},{"õ","o"},{"ø","o"},
    {"Ó","o"},{"Ò","o"},{"Ö","o"},{"Ô","o"},{"Õ","o"},{"Ø","o"},
    {"ō","o"},{"Ō","o"},{"ŏ","o"},{"Ŏ","o"},{"ơ","o"},
    // U
    {"ú","u"},{"ù","u"},{"ü","u"},{"û","u"},
    {"Ú","u"},{"Ù","u"},{"Ü","u"},{"Û","u"},
    {"ū","u"},{"Ū","u"},{"ŭ","u"},{"Ŭ","u"},{"ư","u"},{"ǔ","u"},
    // Y
    {"ý","y"},{"Ý","y"},{"ŷ","y"},
    // C
    {"ç","c"},{"Ç","c"},{"ć","c"},{"č","c"},{"Č","c"},
    // N
    {"ñ","n"},{"Ñ","n"},{"ń","n"},{"ņ","n"},
    // S
    {"ś","s"},{"Ś","s"},{"ş","s"},{"Ş","s"},{"š","s"},{"Š","s"},{"ș","s"},
    // Z
    {"ź","z"},{"Ż","z"},{"ž","z"},{"Ž","z"},
    // L
    {"ł","l"},{"Ł","l"},
    // G
    {"ğ","g"},{"Ğ","g"},
    // T
    {"ţ","t"},{"Ț","t"},{"ț","t"},
    // D
    {"ð","d"},{"Đ","d"},{"đ","d"},
    // Vocales compuestas
    {"æ","ae"},{"Æ","ae"},
    {"œ","oe"},{"Œ","oe"}
};
// Stopword que se eliminaran
unordered_set<string> stopwords = {
    "the", "and", "his", "her", "that", "with", "him", "for", "she", "but",
    "who", "they", "from", "has", "their", "when", "are", "after", "out",
    "into", "them", "was", "not", "one", "which", "then", "about", "while",
    "will", "this", "have", "where", "all", "had", "also", "before", "been",
    "other", "only", "over", "can", "himself", "there", "during", "now",
    "down", "both", "next", "more", "does", "because", "own", "some",
    "what", "again", "just", "each", "its", "against", "between", "would",
    "same", "still", "how", "even", "were", "along", "under", "like",
    "well", "once", "until", "around", "too", "very", "never", "much",
    "than", "without", "though", "going", "found", "instead", "behind",
    "upon", "since", "including", "many", "few", "inside", "outside",
    "everyone", "could", "cannot", "others", "unable", "although", "most",
    "given", "seeing", "high", "alone", "front", "put", "did", "told",
    "saying", "such", "enough", "already", "every", "these", "something",
    "continue", "point", "further", "form", "cause", "seems"
};

// Pasa los strings a minuscula. Utiliza la libreria algorithm
string aMinusculas(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    return texto;
}

// Esta función no hace traducción automática ni detección real de idioma simplemente descarta caracteres que no reconoce.
string limpiarTextoAvanzado(const string& s, const vector<string>& parentesisProhibidos, const unordered_set<string>& palabrasProhibidas) {
    string temp = "";

    // Manejo de [] y ()
    for (size_t i = 0; i < s.length(); ) {
        if (s[i] == '[') {
            size_t start = i + 1;
            size_t end = start;
            while (end < s.length() && s[end] != ']') end++;
            
            string inside = s.substr(start, end - start);
             // Verificamos si adentro solo hay números.
            bool soloNumeros = !inside.empty();
            for (char c : inside) {
                if (!isdigit((unsigned char)c) && c != ' ' && c != ',') {
                    soloNumeros = false;
                    break;
                }
            }
            // Si NO son solo números, conservamos el contenido
            if (!soloNumeros) {
                temp += inside;
            }
            // Si eran solo números, no sumamos nada a 'temp' (se elimina el contenido y los [])
            i = end;
            if (i < s.length()) i++;
        }
        else if (s[i] == '(') {
            size_t start = i + 1;
            size_t end = start;
            while (end < s.length() && s[end] != ')') end++;
            string inside = s.substr(start, end - start);

            string lower_inside = aMinusculas(inside);
            bool descartar = false;
            // Si encontramos palabras prohibidas dentro del paréntesis se elimina todo el contenido.
            for (const string& prohibida : parentesisProhibidos) {
                if (lower_inside.find(prohibida) != string::npos) {
                    descartar = true;
                    break;
                }
            }
            if (!descartar) {
                temp += inside;
            }
            i = end;
            if (i < s.length()) i++;
        } else {
            temp += s[i];
            i++;
        }
    }

    //Procesamiento letra a letra: minúsculas, validación UTF-8, reemplazo acentos y filtrados de otros idiomas.
    string textoFinal = "";
    string palabraActual = "";
    bool palabraValida = true;

    auto procesarPalabra = [&]() {
        if (!palabraActual.empty()) {
            // Si la palabra no tiene caracteres rusos/chinos y no es una de las palabras prohibidas ("director")
            if (palabraValida && palabrasProhibidas.find(palabraActual) == palabrasProhibidas.end()) {
                if (!textoFinal.empty()) textoFinal += " ";
                textoFinal += palabraActual;
            }
            palabraActual = "";
            palabraValida = true;
        }
    };
     // Lectura UTF-8 (necesariamente por carácter)
    for (size_t i = 0; i < temp.length(); ) {
        unsigned char byte = temp[i];
        // ASCII normal
        if (byte < 0x80) {
            char ch = temp[i];
            if (isalnum(ch)) {
                palabraActual += tolower(ch);
                i++;
            } else {
                procesarPalabra();
                i++;
            }
        // UTF-8 multibyte
        } else {
            int len = 0;
            if ((byte & 0xE0) == 0xC0) len = 2;
            else if ((byte & 0xF0) == 0xE0) len = 3;
            else if ((byte & 0xF8) == 0xF0) len = 4;
            else { i++; continue; }

            string c = temp.substr(i, len);
            i += len;

            auto it = accents.find(c);
            // Si conocemos el carácter, lo reemplazamos, de lo contrario se descarta la palabra entera.
            if (it != accents.end()) {
                palabraActual += it->second;
            } else {
                palabraValida = false;
            }
        }
    }
    procesarPalabra();
    return textoFinal;
}

// Limpia la columna Title y borra todo lo que este entre parentesis si estan las palabras film o part dentro
string limpiarTitulo(const string& s) {
    return limpiarTextoAvanzado(s, {"film", "part"}, {});
}

// Limpia la columna Director y borra la palabra director
string limpiarDirector(const string& s) {
    return limpiarTextoAvanzado(s, {}, {"director"});
}

// Se limpia la columna Origin/Ethnicity
string limpiarOrigen(const string& s) {
    string origenFinal = "";
    string palabraActual = "";
    auto procesarPalabra = [&]() {
        if (!palabraActual.empty()) {
            if (!origenFinal.empty()) origenFinal += " ";
            origenFinal += palabraActual;
            palabraActual = "";
        }
    };
    for (char ch : s) {
        if (isalpha((unsigned char)ch)) { palabraActual += tolower((unsigned char)ch); }
        else { procesarPalabra(); }
    }
    procesarPalabra();
    return origenFinal;
}

// Se limpia la columna Cast y se borran las palabras director y screenplay
string limpiarCast(const string& s) {
    return limpiarTextoAvanzado(s, {}, {"director", "screenplay"});
}


// Esta es la limpieza genérica que se uso para las columnas Genre y Plot.
string normalizarYLimpiar(const string& s) {
    string res;
    // Reservo memoria desde el inicio para evitar reallocations innecesarios.
    res.reserve(s.size());
    // Esta bandera indica si actualmente estoy dentro de [ ].
    bool enCorchetes = false;
    string bufferCorchetes = "";
    // Recorro el texto carácter por carácter.
    for (size_t i = 0; i < s.size(); ) {
        unsigned char byte = s[i];
        // Si es un carácter ASCII normal entro acá.
        if ((byte & 0x80) == 0) {
            char ch = s[i];
            if (ch == '[') {
                enCorchetes = true;
                bufferCorchetes = "";
                i++; continue;
            }
            else if (ch == ']') {
                enCorchetes = false;
                bool esSoloNumero = true;
                // Verifico si dentro del corchete solo había números.
                for (char b : bufferCorchetes) {
                    if (b != ' ' && !isdigit((unsigned char)b)) {
                        esSoloNumero = false; break;
                    }
                }
                // Ignoro referencias típicas de Wikipedia porque meten ruido.
                if (!(esSoloNumero || bufferCorchetes == "clarification needed" ||
                      bufferCorchetes == "citation needed" || bufferCorchetes == "dead link" ||
                      bufferCorchetes == "better source needed")) {
                    res += " " + bufferCorchetes + " ";
                }
                i++; continue;
            }
            char procesado;
            // Solo conservo letras y números.
            if (isalnum((unsigned char)ch)) { procesado = tolower((unsigned char)ch); }
            else { procesado = ' '; }
            // Si estoy dentro de corchetes guardo en el buffer temporal.
            if (enCorchetes) bufferCorchetes += procesado;
            else res += procesado;
            i++;
        }
        else {
            int len = 0;
            // Detecto cuántos bytes ocupa el carácter.
            if ((byte & 0xE0) == 0xC0) len = 2;
            else if ((byte & 0xF0) == 0xE0) len = 3;
            else if ((byte & 0xF8) == 0xF0) len = 4;
            // Si el formato es inválido simplemente avanzo.
            else { i++; continue; }
            // Extraigo el carácter UTF-8 completo.
            string c = s.substr(i, len);
            i += len;
            string procesado = "";
            // Busco el carácter en el mapa de acentos.
            auto it = accents.find(c);
            // Si existe, reemplazo por su versión normalizada.
            if (it != accents.end()) procesado = it->second;

            if (enCorchetes) bufferCorchetes += procesado;
            else res += procesado;
        }
    }
    // Caso borde por si el texto termina con un corchete abierto.
    if (enCorchetes) res += " " + bufferCorchetes;
    //Yeii
    return res;
}


// Esta función elimina stopwords del plot para reducir ruido en el motor de búsqueda. Palabras como "the", "and", "with", etc. aparecen demasiadas veces y terminan afectando el ranking porque todas las películas comparten esas palabras.
string filtrarStopwords(const string& textoLimpio) {
    stringstream ss(textoLimpio);
    string palabra, resultado = "";
    bool primero = true;
    while (ss >> palabra) {
        if (stopwords.find(palabra) == stopwords.end()) {
            if (!primero) resultado += " ";
            resultado += palabra;
            primero = false;
        }
    }
    return resultado;
}




// --- EXPORTACIÓN A CSV SIN COMILLAS ---
//Esta función exporta toda la data ya limpia a un CSV y me sirvió bastante para debugging porque podía revisar manualmente cómo estaba quedando cada columna después de la limpieza.
void exportarDataLimpiaCSV(const unordered_map<int, Movie>& pelis, const string& nombreArchivo, std::unordered_map<int, DataLimpia>& datalimpia) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo crear el archivo " << nombreArchivo << endl;
        return;
    }
    archivo << "Release Year,Title,Origin/Ethnicity,Director,Cast,Genre,Plot\n";
    unordered_map<int, DataLimpia> limpiaA;
    for (const auto& [id, movie] : pelis) {
        string year     = movie.getYear();
        string title    = limpiarTitulo(movie.getTtitle());
        string origin   = limpiarOrigen(movie.getOrigin());
        string director = limpiarDirector(movie.getDirector()); 

        string cast     = limpiarCast(movie.getCast());
        string genre    = normalizarYLimpiar(movie.getGenre());

        string plotRaw  = normalizarYLimpiar(movie.getPlot());
        string plotFin  = filtrarStopwords(plotRaw);

        archivo << year << ","
                << title << ","
                << origin << ","
                << director << ","
                << cast << ","
                << genre << ","
                << plotFin << "\n";
        DataLimpia limpia(title,year,origin,director,cast,genre,plotFin);
        limpiaA[id] = limpia;

    }
    datalimpia = limpiaA;

    archivo.close();
    cout << "  -> Archivo CSV limpio exportado: " << nombreArchivo << endl;
}
