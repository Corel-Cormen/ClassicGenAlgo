#pragma once

#include "SelectionAlgoBase.hpp"

class RandomCore;
struct UiData;

class BestSelectionAlgo : public SelectionAlgoBase<BestSelectionAlgo>
{
public:
    bool select_impl(GA::Types::GenomePopulation &genomeVec, const UiData &uiData);

    static constexpr QStringView getAlgoName_impl() { return algoName; }

private:
    static constexpr QStringView algoName{u"Best selection"};
};
