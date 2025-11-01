#pragma once

#include "PopulationFabricBase.hpp"

class RandomCore;
struct UiData;

class RandomPopulationFabric : public PopulationFabricBase<RandomPopulationFabric>
{
public:
    RandomPopulationFabric(const RandomCore &randomRef);

    bool generate_impl(GA::Types::GenomePopulation &genomeVec,
                       const UiData &uiData);

private:
    const RandomCore &random;
};
