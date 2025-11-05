#include "RandomPopulationFabric.hpp"
#include "UiData.h"

RandomPopulationFabric::RandomPopulationFabric(const RandomCore &randomRef) :
    PopulationFabricBase(),
    random{randomRef}
{}

bool RandomPopulationFabric::generate_impl(GA::Types::GenomePopulation &genomeVec,
                                           const UiData &uiData)
{
    bool status = true;

    genomeVec.clear();
    genomeVec.resize(uiData.populationQuantity);
    for (size_t idx = 0U; idx < static_cast<size_t>(uiData.populationQuantity); ++idx)
    {
        genomeVec[idx].point.reserve(uiData.functionDimension);
        for (size_t i = 0U; i < static_cast<size_t>(uiData.functionDimension); ++i)
        {
            Genome genome;
            if (!genome.createGenom(uiData.minSearchRange,
                                    uiData.maxSearchRange,
                                    uiData.precission))
            {
                status = false;
                break;
            }
            genome.initGenom(random);
            genomeVec[idx].point.push_back(std::move(genome));
        }
    }

    return status;
}
