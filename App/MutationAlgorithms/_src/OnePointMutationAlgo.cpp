#include "OnePointMutationAlgo.hpp"

OnePointMutationAlgo::OnePointMutationAlgo(RandomCore &randomRef) :
    MutationDecider<OnePointMutationAlgo>(randomRef)
{}

void OnePointMutationAlgo::mutation_impl(Genome &genome)
{
    size_t mutationIdx = random.randNumber<size_t>(0, genome.size());
    genome[mutationIdx].flip();
}
