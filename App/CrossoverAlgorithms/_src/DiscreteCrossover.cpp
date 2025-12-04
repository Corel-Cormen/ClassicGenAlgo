#include "CommonFunctions.hpp"
#include "DiscreteCrossover.hpp"
#include "RandomCore.hpp"

DiscreteCrossover::DiscreteCrossover(const RandomCore &randomRef) :
    CrossoverIndexBase(),
    random{randomRef}
{}

bool DiscreteCrossover::crossover_impl(const UiData &uiData,
                                       const GA::Types::GenomePoint &g1,
                                       const GA::Types::GenomePoint &g2,
                                       GA::Types::GenomePoint &out1,
                                       GA::Types::GenomePoint &out2)
{
    constexpr qreal probabilityThreshold = 0.5;

    (void) uiData;
    out1.point = g1.point;
    (void) out2;

    for (size_t pAxis = 0; pAxis < g1.point.size(); ++pAxis)
    {
        for (size_t idx = 0; idx < g1.point[pAxis].size(); ++idx)
        {
            const qreal crossProbability = random.randUnit<qreal>();
            if (CommonFunc::moreThan(crossProbability, probabilityThreshold))
            {
                out1.point[pAxis][idx] = g2.point[pAxis][idx];
            }
        }
    }

    return true;
}
