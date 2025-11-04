#include "BestSelectionAlgo.hpp"
#include "UiData.h"

bool BestSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
                                    const UiData &uiData)
{
    size_t nthSize = 0;
    if(uiData.eliteStrategyEnable)
    {
        populationElite(genomeVec, uiData.eliteStrategyQuantity);
        nthSize = uiData.eliteStrategyQuantity;
    }

    std::nth_element(genomeVec.begin() + nthSize, genomeVec.begin() + nthSize, genomeVec.end(),
                     [](const auto& a, const auto& b) { return CommonFunc::lessThan(a.value, b.value); });

    genomeVec.erase(genomeVec.begin() + uiData.selectAlgoPopulationQuantity, genomeVec.end());

    return true;
}
