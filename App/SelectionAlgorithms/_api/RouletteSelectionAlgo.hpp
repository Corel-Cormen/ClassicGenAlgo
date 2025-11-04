#pragma once

#include "SelectionAlgoBase.hpp"

class RandomCore;
struct UiData;

class RouletteSelectionAlgo : public SelectionAlgoBase<RouletteSelectionAlgo>
{
public:
    RouletteSelectionAlgo(RandomCore &randomRef);

    bool select_impl(GA::Types::GenomePopulation &genomeVec, const UiData &uiData);

    static constexpr SelectionAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr SelectionAlgoId algoId{SelectionAlgoId::ROULETTE_SELECTION};

    RandomCore &random;

    static std::vector<qreal> createValVec(const GA::Types::GenomePopulation &genomeVec);
};
