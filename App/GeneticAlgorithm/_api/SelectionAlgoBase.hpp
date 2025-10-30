#pragma once

#include <algorithm>

#include <QStringView>

#include "GeneticAlgorithmTypes.hpp"
#include "CommonFunctions.hpp"

struct UiData;

template <typename Base>
class SelectionAlgoBase
{
public:
    bool select(GA::Types::GenomePopulation &genomVec, const UiData& uiData)
    {
        return static_cast<Base*>(this)->select_impl(genomVec, uiData);
    }

    static constexpr QStringView getAlgoName() { return Base::getAlgoName_impl(); }

protected:
    static void populationScore(GA::Types::GenomePopulation &genomeVec)
    {
        std::sort(genomeVec.begin(), genomeVec.end(),
                  [](const auto &a, const auto &b) {
                      return CommonFunc::lessThan(a.value, b.value);
                  });
    }
};
