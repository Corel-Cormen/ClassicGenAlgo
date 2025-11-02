#pragma once

#include <algorithm>

#include "GeneticAlgorithmTypes.hpp"
#include "CommonFunctions.hpp"

struct UiData;

enum class SelectionAlgoId : quint8
{
    BEST_SELECTION = 0,
    WORST_SELECTION,
};

template <typename Base>
class SelectionAlgoBase
{
public:
    bool select(GA::Types::GenomePopulation &genomVec, const UiData& uiData)
    {
        return static_cast<Base*>(this)->select_impl(genomVec, uiData);
    }

    static constexpr SelectionAlgoId getAlgoName() { return Base::getAlgoName_impl(); }

protected:
    static void populationScore(GA::Types::GenomePopulation &genomeVec)
    {
        std::sort(genomeVec.begin(), genomeVec.end(),
                  [](const auto &a, const auto &b) {
                      return CommonFunc::lessThan(a.value, b.value);
                  });
    }
};
