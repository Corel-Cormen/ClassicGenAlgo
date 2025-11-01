#pragma once

#include "SelectionAlgoBase.hpp"

class RandomCore;
struct UiData;

class WorstSelectionAlgo : public SelectionAlgoBase<WorstSelectionAlgo>
{
public:
    bool select_impl(GA::Types::GenomePopulation &genomeVec, const UiData &uiData);

    static constexpr QStringView getAlgoName_impl() { return algoName; }

private:
    static constexpr QStringView algoName{u"Worst selection"};
};
