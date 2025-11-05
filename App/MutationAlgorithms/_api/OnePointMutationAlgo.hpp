#pragma once

#include "MutationAlgoBase.hpp"

class RandomCore;
struct UiData;

class OnePointMutationAlgo : public MutationAlgoBase<OnePointMutationAlgo>
{
public:
    OnePointMutationAlgo(const RandomCore &randomRef);

    bool mutation_impl(GA::Types::GenomePopulation &genomeVec,
                       const UiData &uiData);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::ONE_POINT_MUTATION};

    const RandomCore &random;
};
