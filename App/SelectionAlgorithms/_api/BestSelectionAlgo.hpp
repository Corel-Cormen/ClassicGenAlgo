#pragma once

#include "SelectionAlgoBase.hpp"

struct UiData;

class BestSelectionAlgo : public SelectionAlgoBase<BestSelectionAlgo>
{
public:
    bool select_impl(GA::Types::GenomePopulation &genomeVec, const UiData &uiData);

    static constexpr SelectionAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr SelectionAlgoId algoId{SelectionAlgoId::BEST_SELECTION};
};
