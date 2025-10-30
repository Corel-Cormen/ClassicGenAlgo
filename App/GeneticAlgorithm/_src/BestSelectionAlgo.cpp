#include "BestSelectionAlgo.hpp"
#include "UiData.h"

bool BestSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
                                    const UiData &uiData)
{
    bool status = false;

    constexpr size_t minimumPopulation = 2U;
    const size_t selectPopulation = static_cast<size_t>(
        static_cast<qreal>(genomeVec.size()) *
        (static_cast<qreal>(uiData.selectAlgoBestPercentPopulation)/100.0));

    if (selectPopulation >= minimumPopulation)
    {
        populationScore(genomeVec);
        genomeVec.erase(genomeVec.begin() + selectPopulation, genomeVec.end());
        status = true;
    }

    return status;
}
