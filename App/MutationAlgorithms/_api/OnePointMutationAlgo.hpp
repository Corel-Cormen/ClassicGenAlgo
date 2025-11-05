#pragma once

#include "MutationDecider.hpp"

class RandomCore;

class OnePointMutationAlgo : public MutationDecider<OnePointMutationAlgo>
{
public:
    explicit OnePointMutationAlgo(RandomCore &randomRef);

    void mutation_impl(Genome &genome);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::ONE_POINT_MUTATION};
};
