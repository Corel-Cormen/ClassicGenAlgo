#include "InwerseMutationAlgo.hpp"

InwerseMutationAlgo::InwerseMutationAlgo(const RandomCore &randomRef) :
    random{randomRef}
{}

bool InwerseMutationAlgo::mutation_impl(GA::Types::GenomePopulation &genomeVec,
                                        const UiData &uiData)
{
    return true;
}
