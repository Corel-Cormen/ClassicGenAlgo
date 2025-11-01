#pragma once

#include "GeneticAlgorithmTypes.hpp"

struct UiData;

template <typename Base>
class PopulationFabricBase
{
public:
    bool generate(GA::Types::GenomePopulation &genomVec,
                  const UiData &uiData)
    {
        return static_cast<Base*>(this)->generate_impl(genomVec, uiData);
    }
};
