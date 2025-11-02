#pragma once

#include "CrossoverIndexBase.hpp"

class RandomCore;

class DiscreteCrossover : public CrossoverIndexBase<DiscreteCrossover>
{
public:
    DiscreteCrossover(const RandomCore &randomRef);

    bool crossover_impl(const UiData &uiData,
                        const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out1,
                        GA::Types::GenomePoint &out2);

    static constexpr CrossoverAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr CrossoverAlgoId algoId{CrossoverAlgoId::DISCRETE};

    const RandomCore &random;
};
