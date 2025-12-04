#pragma once

#include "GeneticAlgorithmTypes.hpp"

struct UiData;

enum class CrossoverAlgoId : quint8
{
    SINGLE_POINT = 0,
    TWO_POINT,
    UNIFORM,
    DISCRETE,

    ARITMETIC,
    LINEAR,
    BLEND_ALPHA,
    BLEND_ALPHA_BETA,
    AVERAGING,
};

template <typename Base>
class CrossoverAlgoBase
{
public:
    bool crossover(GA::Types::GenomePopulation &genomVec, const UiData& uiData)
    {
        return static_cast<Base*>(this)->crossover_impl(genomVec, uiData);
    }

    static constexpr CrossoverAlgoId getAlgoName() { return Base::getAlgoName_impl(); }
};
