#pragma once

#include <algorithm>

#include "GeneticAlgorithmTypes.hpp"
#include "CommonFunctions.hpp"

struct UiData;

enum class SelectionAlgoId : quint8
{
    BEST_SELECTION = 0,
    ROULETTE_SELECTION,
    TOURNAMENT_SELECTION,
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
    static void populationElite(GA::Types::GenomePopulation &genomeVec, size_t size)
    {
        std::nth_element(genomeVec.begin(), genomeVec.begin() + size, genomeVec.end(),
            [](const auto& a, const auto& b) { return CommonFunc::lessThan(a.value, b.value); });
    }
};
