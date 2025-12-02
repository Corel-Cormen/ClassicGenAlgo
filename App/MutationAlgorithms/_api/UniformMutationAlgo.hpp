#pragma once

#include "MutationDecider.hpp"

class RandomCore;

class UniformMutationAlgo : public MutationDecider<UniformMutationAlgo>
{
public:
    explicit UniformMutationAlgo(RandomCore &randomRef);

    void mutation_impl(Genome &genome);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::UNIFORM_MUTATION};

    RandomCore &random;
};
