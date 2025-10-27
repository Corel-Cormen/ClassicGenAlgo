#pragma once

#include "GeneticAlgorithmTypes.hpp"

template <typename Base>
class EvaluateAlgoBase
{
public:
    bool evaluate(GA::Types::GenomePopulation &genomVec)
    {
        return static_cast<Base*>(this)->evaluate_impl(genomVec);
    }
};
