#pragma once

#include "CrossoverIndexBase.hpp"

class PyFunctionEvaluateAlgo;

class LinearCrossover : public CrossoverIndexBase<LinearCrossover>
{
public:
    LinearCrossover(PyFunctionEvaluateAlgo &evaluatorRef);

    bool crossover_impl(const UiData &uiData,
                        const GA::Types::GenomePoint &g1,
                        const GA::Types::GenomePoint &g2,
                        GA::Types::GenomePoint &out1,
                        GA::Types::GenomePoint &out2);

    static constexpr CrossoverAlgoId getAlgoName_impl() { return algoId; }

private:
    static constexpr CrossoverAlgoId algoId{CrossoverAlgoId::LINEAR};

    PyFunctionEvaluateAlgo &evaluator;

    static qreal calcZ(const qreal x, const qreal y);
    static qreal calcV(const qreal x, const qreal y);
    static qreal calcW(const qreal x, const qreal y);

    static std::vector<GA::Types::GenomePopulation> createPopulation(const GA::Types::GenomePoint &p1,
                                                                     const GA::Types::GenomePoint &p2,
                                                                     const GA::Types::GenomePoint &p3);
    static std::pair<size_t, size_t> findMins(const std::vector<GA::Types::GenomePopulation> &points);
};
