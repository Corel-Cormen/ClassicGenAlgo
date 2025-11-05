#pragma once

#include "MutationAlgoBase.hpp"

class RandomCore;
struct UiData;

class TwoPointMutationAlgo : public MutationAlgoBase<TwoPointMutationAlgo>
{
public:
    TwoPointMutationAlgo(const RandomCore &randomRef);

    bool mutation_impl(GA::Types::GenomePopulation &genomeVec,
                       const UiData &uiData);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::TWO_POINT_MUTATION};

    const RandomCore &random;
};
