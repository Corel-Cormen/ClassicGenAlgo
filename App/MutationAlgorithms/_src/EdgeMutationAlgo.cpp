#include "EdgeMutationAlgo.hpp"

EdgeMutationAlgo::EdgeMutationAlgo(RandomCore &randomRef) :
    MutationDecider<EdgeMutationAlgo>(randomRef)
{}

void EdgeMutationAlgo::mutation_impl(Genome &genome)
{
    const quint8 mutationEdge = random.randUnit<quint8>();
    if (mutationEdge == 0U)
    {
        genome[0].flip();
    }
    else
    {
        genome[genome.size()-1].flip();
    }
}
