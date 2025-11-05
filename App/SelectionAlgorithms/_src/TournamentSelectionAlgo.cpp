#include "TournamentSelectionAlgo.hpp"
#include "UiData.h"

bool TournamentSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
                                          const UiData &uiData)
{
    GA::Types::GenomePopulation result{};
    result.reserve(genomeVec.size());

    if(uiData.eliteStrategyEnable)
    {
        populationElite(genomeVec, uiData.eliteStrategyQuantity);
        for (size_t i = 0U; i < uiData.eliteStrategyQuantity; ++i)
        {
            result.push_back(genomeVec[i]);
        }
        genomeVec.erase(genomeVec.begin(), genomeVec.begin() + uiData.eliteStrategyQuantity);
    }

    while(result.size() < uiData.selectAlgoPopulationQuantity)
    {
        std::vector<bool> tornamentWinIndex = tournament(genomeVec, uiData);

        for(size_t idx = 0U; idx < tornamentWinIndex.size(); ++idx)
        {
            if (tornamentWinIndex[idx] == true)
            {
                result.push_back(genomeVec[idx]);
            }
        }

        auto it = std::remove_if(genomeVec.begin(), genomeVec.end(),
                                 [&, i = 0](const auto&) mutable {
                                     return tornamentWinIndex[i++];
                                 });
        genomeVec.erase(it, genomeVec.end());
    }

    if(result.size() > uiData.selectAlgoPopulationQuantity)
    {
        result.erase(result.begin() + uiData.selectAlgoPopulationQuantity, result.end());
    }

    genomeVec = result;

    return true;
}

std::vector<bool> TournamentSelectionAlgo::tournament(
    const GA::Types::GenomePopulation &genomeVec,
    const UiData &uiData)
{
    std::vector<bool> tournamentWinIndex(genomeVec.size(), false);
    for (size_t idx = 0U; idx < genomeVec.size(); idx += uiData.tournamentPopulationQuantity)
    {
        auto minIt = std::min_element(genomeVec.begin() + idx,
                                      (idx + uiData.tournamentPopulationQuantity < genomeVec.size()) ?
                                          genomeVec.begin() + idx + uiData.tournamentPopulationQuantity :
                                          genomeVec.end(),
                                      [](const GA::Types::GenomePoint& a, const GA::Types::GenomePoint& b) {
                                          return CommonFunc::lessEqThan(a.value, b.value);
                                      });

        if (minIt != genomeVec.end())
        {
            size_t minIdx = std::distance(genomeVec.begin(), minIt);
            tournamentWinIndex[minIdx] = true;
        }
    }
    return tournamentWinIndex;
}
