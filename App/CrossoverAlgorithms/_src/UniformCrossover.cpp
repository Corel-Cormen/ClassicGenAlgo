#include "UniformCrossover.hpp"

#include "RandomCore.hpp"
#include "CommonFunctions.hpp"

UniformCrossover::UniformCrossover(const RandomCore &randomRef) :
    CrossoverIndexBase(),
    random{randomRef}
{}

bool UniformCrossover::crossover_impl(const UiData &uiData,
                                      const GA::Types::GenomePoint &g1,
                                      const GA::Types::GenomePoint &g2,
                                      GA::Types::GenomePoint &out1,
                                      GA::Types::GenomePoint &out2)
{
    out1.point = g1.point;
    out2.point = g2.point;

    for (size_t pAxis = 0; pAxis < g1.point.size(); ++pAxis)
    {
        for (size_t idx = 0; idx < g1.point[pAxis].size(); ++idx)
        {
            const qreal crossProbability = random.randUnit<qreal>();
            if (CommonFunc::moreEqThan(crossProbability, uiData.crossingPropablity))
            {
                out1.point[pAxis][idx] = g2.point[pAxis][idx];
                out2.point[pAxis][idx] = g1.point[pAxis][idx];
            }
        }
    }

    return true;
}
