#include "UniformMutationAlgo.hpp"

UniformMutationAlgo::UniformMutationAlgo(RandomCore &randomRef) :
    MutationDecider<UniformMutationAlgo>(randomRef),
    random{randomRef}
{}

void UniformMutationAlgo::mutation_impl(Genome &genome)
{
    qreal newValue = random.randNumber(genome.getMinVal(), genome.getMaxVal());
    genome.setValue(newValue);
}
