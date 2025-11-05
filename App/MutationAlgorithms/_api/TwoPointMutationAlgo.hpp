#pragma once

#include "MutationDecider.hpp"

class RandomCore;

class TwoPointMutationAlgo : public MutationDecider<TwoPointMutationAlgo>
{
public:
    explicit TwoPointMutationAlgo(RandomCore &randomRef);

    void mutation_impl(Genome &genome);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::TWO_POINT_MUTATION};
};
