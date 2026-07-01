#include "RankingStrategy.h"

vector<int> RankingPorScoreStrategy::ordenar(const unordered_map<int, double>& scores) const {
    vector<Resultado> orden;
    orden.reserve(scores.size());

    for (const auto& [id, score] : scores) {
        orden.push_back({id, score});
    }

    sort(orden.begin(), orden.end(), [](const Resultado& a, const Resultado& b) {
        if (a.score == b.score) {
            return a.id < b.id;
        }
        return a.score > b.score;
    });

    vector<int> respuesta;
    respuesta.reserve(orden.size());
    for (const auto& resultado : orden) {
        respuesta.push_back(resultado.id);
    }
    return respuesta;
}
