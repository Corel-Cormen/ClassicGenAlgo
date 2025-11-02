#pragma once

#include "CrossoverIndexBase.hpp"

class RandomCore;

class SinglePointCrossover : public CrossoverIndexBase<SinglePointCrossover>
{
public:
    SinglePointCrossover(const RandomCore &randomRef);

    bool crossover_impl(const UiData &uiData,
                        const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out1,
                        GA::Types::GenomePoint &out2);

    static constexpr QStringView getAlgoName_impl() { return algoName; }

private:
    static constexpr QStringView algoName{u"Single point"};

    const RandomCore &random;
};
