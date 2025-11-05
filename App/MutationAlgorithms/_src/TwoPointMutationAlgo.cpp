#include "TwoPointMutationAlgo.hpp"

TwoPointMutationAlgo::TwoPointMutationAlgo(RandomCore &randomRef) :
    MutationDecider<TwoPointMutationAlgo>(randomRef)
{}

void TwoPointMutationAlgo::mutation_impl(Genome &genome)
{
    const auto mutationIdx = random.randTwo<size_t>(0, genome.size());
    genome[mutationIdx->first] = !genome[mutationIdx->first];
    genome[mutationIdx->second] = !genome[mutationIdx->second];
}
