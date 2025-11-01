#include "BestSelectionAlgo.hpp"
#include "UiData.h"

bool BestSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
                                    const UiData &uiData)
{
    bool status = false;

    constexpr size_t minimumPopulation = 2U;
    if (uiData.selectAlgoPopulationQuantity >= minimumPopulation)
    {
        populationScore(genomeVec);
        genomeVec.erase(genomeVec.begin() + uiData.selectAlgoPopulationQuantity, genomeVec.end());
        status = true;
    }

    return status;
}
