#pragma once

#include "CrossoverIndexBase.hpp"

class RandomCore;

class ArithmeticCrossover : public CrossoverIndexBase<ArithmeticCrossover>
{
public:
    ArithmeticCrossover(const RandomCore &randomRef);

    bool crossover_impl(const UiData &uiData,
                        const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out1,
                        GA::Types::GenomePoint &out2);

    static constexpr CrossoverAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr CrossoverAlgoId algoId{CrossoverAlgoId::ARITMETIC};

    const RandomCore &random;

    qreal calculateValue(const qreal x, const qreal y, const qreal alpha);
};
