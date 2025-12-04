#include "AveragingCrossover.hpp"

AveragingCrossover::AveragingCrossover() :
    CrossoverIndexBase()
{}

bool AveragingCrossover::crossover_impl(const UiData &uiData,
                                        const GA::Types::GenomePoint &g1,
                                        const GA::Types::GenomePoint &g2,
                                        GA::Types::GenomePoint &out1,
                                        GA::Types::GenomePoint &out2)
{
    out1 = g1;

    for (size_t idx = 0U; idx < g1.point.size(); ++idx)
    {
        const qreal newValue = (g1.point[idx].val() + g2.point[idx].val()) / 2.0;
        out1.point[idx].setValue(newValue);
    }

    return true;
}
