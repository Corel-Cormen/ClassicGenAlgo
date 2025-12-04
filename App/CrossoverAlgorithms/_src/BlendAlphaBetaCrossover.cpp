#include "BlendAlphaBetaCrossover.hpp"
#include "CommonFunctions.hpp"
#include "RandomCore.hpp"

BlendAlphaBetaCrossover::BlendAlphaBetaCrossover(const RandomCore &randomRef) :
    CrossoverIndexBase(),
    random{randomRef}
{}

bool BlendAlphaBetaCrossover::crossover_impl(const UiData &uiData,
                                             const GA::Types::GenomePoint &g1,
                                             const GA::Types::GenomePoint &g2,
                                             GA::Types::GenomePoint &out1,
                                             GA::Types::GenomePoint &out2)
{
    (void) uiData;

    qreal alpha = random.randUnit<qreal>();
    qreal beta = random.randUnit<qreal>();
    out1 = g1;
    out2 = g2;

    for (size_t idx = 0U; idx < g1.point.size(); ++idx)
    {
        qreal min = CommonFunc::calcMinElement(g1.point[idx].val(), g2.point[idx].val(), alpha);
        qreal max = CommonFunc::calcMaxElement(g1.point[idx].val(), g2.point[idx].val(), beta);
        min = CommonFunc::alignMinValue(min, g1.point[idx].getMinVal(), g2.point[idx].getMinVal());
        max = CommonFunc::alignMaxValue(max, g1.point[idx].getMaxVal(), g2.point[idx].getMaxVal());

        const qreal newG1 = random.randNumber(min, max);
        const qreal newG2 = random.randNumber(min, max);

        out1.point[idx].setValue(newG1);
        out2.point[idx].setValue(newG2);
    }

    return true;
}
