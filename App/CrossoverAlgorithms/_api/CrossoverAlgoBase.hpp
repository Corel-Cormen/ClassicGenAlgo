#pragma once

#include "GeneticAlgorithmTypes.hpp"

struct UiData;

template <typename Base>
class CrossoverAlgoBase
{
public:
    bool crossover(GA::Types::GenomePopulation &genomVec, const UiData& uiData)
    {
        return static_cast<Base*>(this)->crossover_impl(genomVec, uiData);
    }
};
