#include "WorstSelectionAlgo.hpp"
#include "UiData.h"

bool WorstSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
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
        genomeVec.erase(genomeVec.begin(), genomeVec.end() - selectPopulation);
        status = true;
    }

    return status;
}
