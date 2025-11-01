#include "WorstSelectionAlgo.hpp"
#include "UiData.h"

bool WorstSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
                                    const UiData &uiData)
{
    bool status = false;

    constexpr size_t minimumPopulation = 2U;
    if (uiData.selectAlgoPopulationQuantity >= minimumPopulation)
    {
        populationScore(genomeVec);
        genomeVec.erase(genomeVec.begin(), genomeVec.end() - uiData.selectAlgoPopulationQuantity);
        status = true;
    }

    return status;
}
