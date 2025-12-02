#pragma once

#include "MutationDecider.hpp"

class RandomCore;

class GaussMutationAlgo : public MutationDecider<GaussMutationAlgo>
{
public:
    explicit GaussMutationAlgo(RandomCore &randomRef);

    void mutation_impl(Genome &genome);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::GAUSS_MUTATION};

    RandomCore &random;
};
