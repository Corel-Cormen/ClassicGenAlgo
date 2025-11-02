#include "SinglePointCrossover.hpp"

#include "RandomCore.hpp"

SinglePointCrossover::SinglePointCrossover(const RandomCore &randomRef) :
    CrossoverIndexBase(),
    random{randomRef}
{}

bool SinglePointCrossover::crossover_impl(const UiData &uiData,
                                          const GA::Types::GenomePoint &g1,
                                          const GA::Types::GenomePoint &g2,
                                          GA::Types::GenomePoint &out1,
                                          GA::Types::GenomePoint &out2)
{
    out1.point = g1.point;
    out2.point = g2.point;
    const size_t corssIndex = random.randNumber<size_t>(1U, g1.point[0].size());

    for (size_t pAxis = 0; pAxis < g2.point.size(); ++pAxis)
    {
        for (size_t idx = corssIndex; idx < g2.point[pAxis].size(); ++idx)
        {
            out1.point[pAxis][idx] = g2.point[pAxis][idx];
            out2.point[pAxis][idx] = g1.point[pAxis][idx];
        }
    }

    return true;
}
