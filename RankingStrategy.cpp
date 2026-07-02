#include "RankingStrategy.h"

vector<int> RankingPorScoreStrategy::ordenar(const unordered_map<int, double>& scores) const {
    auto orden = ordenarPorValor(scores);
    vector<int> respuesta;
    respuesta.reserve(orden.size());
    for (const auto& [id, _] : orden) {
        respuesta.push_back(id);
    }
    return respuesta;
}
