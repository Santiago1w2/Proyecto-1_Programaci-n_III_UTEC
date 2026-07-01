
#ifndef PROYECTO_1_PROGRAMACION_III_UTEC_RANKINGSTRATEGY_H
#define PROYECTO_1_PROGRAMACION_III_UTEC_RANKINGSTRATEGY_H

#include "Trie.h"

class RankingStrategy {
public:
    virtual ~RankingStrategy() = default;
    virtual vector<int> ordenar(const unordered_map<int, double>& scores) const = 0;
};

class RankingPorScoreStrategy : public RankingStrategy {
public:
    vector<int> ordenar(const unordered_map<int, double>& scores) const override;
};

#endif
