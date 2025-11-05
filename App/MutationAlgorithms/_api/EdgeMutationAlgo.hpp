#pragma once

#include "MutationDecider.hpp"

class RandomCore;

class EdgeMutationAlgo : public MutationDecider<EdgeMutationAlgo>
{
public:
    explicit EdgeMutationAlgo(RandomCore &randomRef);

    void mutation_impl(Genome &genome);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::EDGE_MUTATION};
};
