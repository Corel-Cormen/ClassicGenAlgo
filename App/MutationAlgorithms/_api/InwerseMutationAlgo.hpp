#pragma once

#include "MutationAlgoBase.hpp"

class RandomCore;
struct UiData;

class InwerseMutationAlgo : public MutationAlgoBase<InwerseMutationAlgo>
{
public:
    InwerseMutationAlgo(const RandomCore &randomRef);

    bool mutation_impl(GA::Types::GenomePopulation &genomeVec,
                       const UiData &uiData);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::INWERSE_MUTATION};

    const RandomCore &random;
};
