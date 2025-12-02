#pragma once

#include "GeneticAlgorithmTypes.hpp"

struct UiData;

enum class MutationAlgoId : quint8
{
    EDGE_MUTATION = 0,
    ONE_POINT_MUTATION,
    TWO_POINT_MUTATION,
    INWERSE_MUTATION,

    UNIFORM_MUTATION,
    GAUSS_MUTATION,
};

template <typename Base>
class MutationAlgoBase
{
public:
    bool mutation(GA::Types::GenomePopulation &genomVec, const UiData& uiData)
    {
        return static_cast<Base*>(this)->mutation_impl(genomVec, uiData);
    }

    static constexpr MutationAlgoId getAlgoName() { return Base::getAlgoName_impl(); }
};
