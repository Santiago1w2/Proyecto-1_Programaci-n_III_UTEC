//
// Created by Pieri on 4/05/2026.
#include "Pprocesamiento.h"
#include <sstream>

unordered_map<string, char> acentos = {
    {"á",'a'}, {"à",'a'}, {"ä",'a'}, {"â",'a'}, {"ã",'a'}, {"å",'a'},
    {"Á",'a'}, {"À",'a'}, {"Ä",'a'}, {"Â",'a'}, {"Ã",'a'}, {"Å",'a'},
    {"ó",'o'}, {"ò",'o'}, {"ö",'o'}, {"ô",'o'}, {"õ",'o'}, {"ø",'o'},
    {"Ó",'o'}, {"Ò",'o'}, {"Ö",'o'}, {"Ô",'o'}, {"Õ",'o'}, {"Ø",'o'},
    {"é",'e'}, {"è",'e'}, {"ë",'e'}, {"ê",'e'},
    {"É",'e'}, {"È",'e'}, {"Ë",'e'}, {"Ê",'e'},
    {"í",'i'}, {"ì",'i'}, {"ï",'i'}, {"î",'i'},
    {"Í",'i'}, {"Ì",'i'}, {"Ï",'i'}, {"Î",'i'},
    {"ú",'u'}, {"ù",'u'}, {"ü",'u'}, {"û",'u'},
    {"Ú",'u'}, {"Ù",'u'}, {"Ü",'u'}, {"Û",'u'},
    {"ç",'c'}, {"Ç",'c'},
    {"ñ",'n'}, {"Ñ",'n'}
};

bool leerFilaCSV(ifstream& file, string& lineaCompleta) {
    string linea;
    if (!getline(file, linea)) return false;

    lineaCompleta = linea;
    int comillas = 0;

    for (char c : linea) {
        if (c == '"') comillas++;
    }

    while (comillas % 2 != 0) {
        if (!getline(file, linea)) break;
        lineaCompleta += "\n" + linea;

        for (char c : linea) {
            if (c == '"') comillas++;
        }
    }
    return true;
}

vector<string> parseCSVLine(const string& linea) {
    vector<string> fila;
    string campo = "";
    bool enComillas = false;

    for (size_t i = 0; i < linea.size(); i++) {
        char c = linea[i];

        if (c == '"') {
            if (enComillas && i + 1 < linea.size() && linea[i + 1] == '"') {
                campo += '"';
                i++;
            } else {
                enComillas = !enComillas;
            }
        }
        else if (c == ',' && !enComillas) {
            fila.push_back(campo);
            campo = "";
        }
        else {
            campo += c;
        }
    }
    fila.push_back(campo);
    return fila;
}

string aMinusculas(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    return texto;
}


string limpiarParentesisBasura(const string& s) {
    string res;
    string buffer;
    bool dentro = false;
    char tipoApertura = 0;

    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            dentro = true;
            tipoApertura = c;
            buffer.clear();
        }
        else if (
            (tipoApertura == '(' && c == ')') ||
            (tipoApertura == '[' && c == ']') ||
            (tipoApertura == '{' && c == '}')
        ) {
            dentro = false;

            if (buffer.size() > 1) {
                size_t posP = buffer.find("P:");
                if (posP != string::npos) {
                    string pinyin;
                    for (size_t i = posP + 2; i < buffer.size(); i++) {
                        if (buffer[i] == ',') break;
                        pinyin += buffer[i];
                    }
                    res += " " + pinyin + " ";
                }
                else {
                    bool tieneDosPuntos = false;
                    for (char x : buffer) {
                        if (x == ':') {
                            tieneDosPuntos = true;
                            break;
                        }
                    }
                    if (!tieneDosPuntos) {
                        res += " " + buffer + " ";
                    }
                }
            }
        }
        else {
            if (dentro) buffer += c;
            else res += c;
        }
    }
    return res;
}

unordered_map<int, string> prepararDataLimpia(const unordered_map<int, Movie>& pelis) {
    unordered_map<int, string> dataLimpia;

    for (const auto& [id, movie] : pelis) {
        string raw = movie.getYear() + " " + movie.getTtitle() + " " +
                     movie.getOrigin() + " " + movie.getDirector() + " " +
                     movie.getGenre() + " " + movie.getPlot();

        string final_str;
        final_str.reserve(raw.size());

        for (size_t i = 0; i < raw.size(); i++) {
            unsigned char c = raw[i];

            if (c < 128) {
                char minuscula = tolower(c);
                if (isalnum(minuscula) || minuscula == ' ') {
                    final_str += minuscula;
                }
            }
            else {
                if ((c >= 0xC0 && c <= 0xDF) && (i + 1 < raw.size())) {
                    string utf8_char = raw.substr(i, 2);
                    auto it = acentos.find(utf8_char);
                    if (it != acentos.end()) {
                        final_str += it->second;
                    }
                    i++;
                }
                else if (c >= 0xE0 && c <= 0xEF) { i += 2; }
                else if (c >= 0xF0 && c <= 0xF7) { i += 3; }
            }
        }
        dataLimpia[id] = final_str;
    }
    return dataLimpia;
}
//
