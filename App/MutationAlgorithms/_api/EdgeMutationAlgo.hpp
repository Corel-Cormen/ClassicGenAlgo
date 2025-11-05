#pragma once

#include "MutationAlgoBase.hpp"

class RandomCore;
struct UiData;

class EdgeMutationAlgo : public MutationAlgoBase<EdgeMutationAlgo>
{
public:
    EdgeMutationAlgo(const RandomCore &randomRef);

    bool mutation_impl(GA::Types::GenomePopulation &genomeVec,
                       const UiData &uiData);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::EDGE_MUTATION};

    const RandomCore &random;
};
