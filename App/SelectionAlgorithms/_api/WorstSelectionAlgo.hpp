#pragma once

#include "SelectionAlgoBase.hpp"

class RandomCore;
struct UiData;

class WorstSelectionAlgo : public SelectionAlgoBase<WorstSelectionAlgo>
{
public:
    bool select_impl(GA::Types::GenomePopulation &genomeVec, const UiData &uiData);

    static constexpr SelectionAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr SelectionAlgoId algoId{SelectionAlgoId::WORST_SELECTION};
};
