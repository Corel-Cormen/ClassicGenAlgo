#include "RandomCore.hpp"
#include "RouletteSelectionAlgo.hpp"
#include "UiData.h"

RouletteSelectionAlgo::RouletteSelectionAlgo(RandomCore &randomRef) :
    SelectionAlgoBase(),
    random{randomRef}
{}

bool RouletteSelectionAlgo::select_impl(GA::Types::GenomePopulation &genomeVec,
                                    const UiData &uiData)
{
    if(uiData.eliteStrategyEnable)
    {
        populationElite(genomeVec, uiData.eliteStrategyQuantity);
    }
    std::vector<qreal> valVec = createValVec(genomeVec);

    const size_t holdPoipulation =(uiData.eliteStrategyEnable) ? uiData.eliteStrategyQuantity : 0U;
    const size_t population = genomeVec.size();
    for (size_t idx = 0U; idx < (population - uiData.selectAlgoPopulationQuantity); ++idx)
    {
        qreal sunPropability = 0.0;
        for (size_t i = holdPoipulation; i < genomeVec.size(); ++i)
        {
            sunPropability += valVec[i];
        }

        size_t removeIdx;
        if (sunPropability != 0.0)
        {
            std::vector<qreal> probabilities;
            for(size_t i = holdPoipulation; i < genomeVec.size(); ++i)
            {
                probabilities.push_back(valVec[i] / sunPropability);
            }
            removeIdx = random.randDistIndex(probabilities) + holdPoipulation;
        }
        else
        {
            removeIdx = genomeVec.size()-1;
        }
        genomeVec.erase(genomeVec.begin()+removeIdx);
        valVec.erase(valVec.begin()+removeIdx);
    }

    return true;
}

std::vector<qreal> RouletteSelectionAlgo::createValVec(const GA::Types::GenomePopulation &genomeVec)
{
    std::vector<qreal> valVac;
    valVac.reserve(genomeVec.size());
    bool scaleFlag = false;
    qreal scaleValue = 0.0;
    for(const auto& genome : genomeVec)
    {
        valVac.push_back(genome.value);
        if (CommonFunc::lessThan(genome.value, scaleValue))
        {
            scaleFlag = true;
            scaleValue = genome.value;
        }
    }
    if (scaleFlag)
    {
        for (auto& val : valVac)
        {
            val -= scaleValue;
        }
    }
    return valVac;
}
