#pragma once

#include <vector>

#include <QtTypes>
#include <QString>

#include "GeneticAlgorithmTypes.hpp"

class AlgoLoggerInterface
{
public:

    virtual ~AlgoLoggerInterface() = default;

    virtual void colectData(const GA::Types::GenomePopulation &population) = 0;

    virtual void reset() = 0;

    virtual QString createResultPath() = 0;

    inline const std::vector<qreal>& standardDeviation() { return stdDevVec; }

    inline const std::vector<qreal>& mean() { return meanVec; }

    inline const std::vector<GA::Types::Points>& min() { return minVec; }

protected:
    std::vector<qreal> stdDevVec{};
    std::vector<qreal> meanVec{};
    std::vector<GA::Types::Points> minVec{};
};
