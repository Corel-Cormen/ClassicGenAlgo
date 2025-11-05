#include "EdgeMutationAlgo.hpp"

EdgeMutationAlgo::EdgeMutationAlgo(const RandomCore &randomRef) :
    random{randomRef}
{}

bool EdgeMutationAlgo::mutation_impl(GA::Types::GenomePopulation &genomeVec,
                                     const UiData &uiData)
{
    return true;
}
