#include "OnePointMutationAlgo.hpp"

OnePointMutationAlgo::OnePointMutationAlgo(const RandomCore &randomRef) :
    random{randomRef}
{}

bool OnePointMutationAlgo::mutation_impl(GA::Types::GenomePopulation &genomeVec,
                                         const UiData &uiData)
{
    return true;
}
