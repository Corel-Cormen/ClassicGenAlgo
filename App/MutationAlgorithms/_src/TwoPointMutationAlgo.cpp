#include "TwoPointMutationAlgo.hpp"

TwoPointMutationAlgo::TwoPointMutationAlgo(const RandomCore &randomRef) :
    random{randomRef}
{}

bool TwoPointMutationAlgo::mutation_impl(GA::Types::GenomePopulation &genomeVec,
                                         const UiData &uiData)
{
    return true;
}
