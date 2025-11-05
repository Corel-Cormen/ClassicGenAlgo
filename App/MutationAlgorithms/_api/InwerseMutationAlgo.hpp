#pragma once

#include "MutationDecider.hpp"

class RandomCore;

class InwerseMutationAlgo : public MutationDecider<InwerseMutationAlgo>
{
public:
    explicit InwerseMutationAlgo(RandomCore &randomRef);

    void mutation_impl(Genome &genome);

    static constexpr MutationAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr MutationAlgoId algoId{MutationAlgoId::INWERSE_MUTATION};
};
