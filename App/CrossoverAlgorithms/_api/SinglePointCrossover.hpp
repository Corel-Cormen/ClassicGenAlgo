#pragma once

#include "CrossoverIndexBase.hpp"

class SinglePointCrossover : public CrossoverIndexBase<SinglePointCrossover>
{
public:
    bool crossover_impl(const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out);
};
