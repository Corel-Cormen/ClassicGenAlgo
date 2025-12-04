#include "CommonFunctions.hpp"
#include "LinearCrossover.hpp"
#include "PyFunctionEvaluateAlgo.hpp"

LinearCrossover::LinearCrossover(PyFunctionEvaluateAlgo &evaluatorRef) :
    CrossoverIndexBase(),
    evaluator{evaluatorRef}
{}

bool LinearCrossover::crossover_impl(const UiData &uiData,
                                     const GA::Types::GenomePoint &g1,
                                     const GA::Types::GenomePoint &g2,
                                     GA::Types::GenomePoint &out1,
                                     GA::Types::GenomePoint &out2)
{
    size_t size = g1.point.size();

    GA::Types::GenomePoint Z;
    GA::Types::GenomePoint V;
    GA::Types::GenomePoint W;
    Z.point.reserve(size);
    V.point.reserve(size);
    W.point.reserve(size);

    for (size_t idx = 0U; idx < size; ++idx)
    {
        Genome gZ = g1.point[idx];
        Genome gV = g1.point[idx];
        Genome gW = g1.point[idx];

        qreal zVal = calcZ(g1.point[idx].val(), g2.point[idx].val());
        qreal vVal = calcV(g1.point[idx].val(), g2.point[idx].val());
        qreal wVal = calcW(g1.point[idx].val(), g2.point[idx].val());

        zVal = CommonFunc::alignValue(zVal, g1.point[idx].getMinVal(), g2.point[idx].getMinVal(),
                                      g1.point[idx].getMaxVal(), g2.point[idx].getMaxVal());
        vVal = CommonFunc::alignValue(vVal, g1.point[idx].getMinVal(), g2.point[idx].getMinVal(),
                                      g1.point[idx].getMaxVal(), g2.point[idx].getMaxVal());
        wVal = CommonFunc::alignValue(wVal, g1.point[idx].getMinVal(), g2.point[idx].getMinVal(),
                                      g1.point[idx].getMaxVal(), g2.point[idx].getMaxVal());

        gZ.setValue(zVal);
        gV.setValue(vVal);
        gW.setValue(wVal);
        Z.point.push_back(std::move(gZ));
        V.point.push_back(std::move(gV));
        W.point.push_back(std::move(gW));
    }

    auto points = createPopulation(Z, V, W);

    bool calcStatus = true;
    for (size_t i = 0; i < points.size(); ++i)
    {
        calcStatus = evaluator.evaluate(points[i]) & calcStatus;
    }

    if (calcStatus)
    {
        out1 = g1;
        out2 = g2;

        auto [min1, min2] = findMins(points);

        for (size_t idx = 0U; idx < size; ++idx)
        {
            out1.point[idx].setValue(points[min1][0].point[idx].val());
            out2.point[idx].setValue(points[min2][0].point[idx].val());
        }
    }

    return calcStatus;
}

qreal LinearCrossover::calcZ(const qreal x, const qreal y)
{
    qreal val = (x/2.0) + (y/2.0);
    return val;
}

qreal LinearCrossover::calcV(const qreal x, const qreal y)
{
    qreal val = ((3.0 * x)/2.0) - (y/2.0);
    return val;
}

qreal LinearCrossover::calcW(const qreal x, const qreal y)
{
    qreal val = (((-1.0) * x)/2.0) - ((3.0 * y)/2.0);
    return val;
}

std::vector<GA::Types::GenomePopulation> LinearCrossover::createPopulation(
    const GA::Types::GenomePoint &p1,
    const GA::Types::GenomePoint &p2,
    const GA::Types::GenomePoint &p3)
{
    GA::Types::GenomePopulation P1Point;
    GA::Types::GenomePopulation P2Point;
    GA::Types::GenomePopulation P3Point;
    P1Point.push_back(p1);
    P2Point.push_back(p2);
    P3Point.push_back(p3);

    std::vector<GA::Types::GenomePopulation> points;
    points.reserve(3U);
    points.push_back(P1Point);
    points.push_back(P2Point);
    points.push_back(P3Point);

    return points;
}

std::pair<size_t, size_t> LinearCrossover::findMins(
    const std::vector<GA::Types::GenomePopulation> &points)
{
    size_t idx0 = 0U;
    size_t idx1 = 1U;
    size_t idx2 = 2U;

    if (CommonFunc::lessThan(points[idx1][0].value, points[idx0][0].value))
    {
        std::swap(idx0, idx1);
    }
    if (CommonFunc::lessThan(points[idx2][0].value, points[idx0][0].value))
    {
        std::swap(idx0, idx2);
    }
    if (CommonFunc::lessThan(points[idx2][0].value, points[idx1][0].value))
    {
        std::swap(idx1, idx2);
    }

    return {idx0, idx1};
}
