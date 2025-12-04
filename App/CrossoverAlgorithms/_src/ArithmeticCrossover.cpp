#include "ArithmeticCrossover.hpp"
#include "RandomCore.hpp"

ArithmeticCrossover::ArithmeticCrossover(const RandomCore &randomRef) :
    CrossoverIndexBase(),
    random{randomRef}
{}

bool ArithmeticCrossover::crossover_impl(const UiData &uiData,
                                         const GA::Types::GenomePoint &g1,
                                         const GA::Types::GenomePoint &g2,
                                         GA::Types::GenomePoint &out1,
                                         GA::Types::GenomePoint &out2)
{
    (void) uiData;

    const qreal alpha = random.randUnit<qreal>();
    out1 = g1;
    out2 = g2;

    for (size_t idx = 0U; idx < g1.point.size(); ++idx)
    {
        const qreal newX = calculateValue(g1.point[idx].val(), g2.point[idx].val(), alpha);
        out1.point[idx].setValue(newX);
        const qreal newY = calculateValue(g2.point[idx].val(), g1.point[idx].val(), alpha);
        out2.point[idx].setValue(newY);
    }

    return true;
}

qreal ArithmeticCrossover::calculateValue(const qreal x, const qreal y, const qreal alpha)
{
    return (alpha * x) + ((1.0 - alpha) * y);
}
