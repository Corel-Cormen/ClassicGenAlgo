#pragma once

#include "AlgoLoggerInterface.hpp"

class AlgoStatisticLogger : public AlgoLoggerInterface
{
public:
    void colectData(const GA::Types::GenomePopulation &population) override;

    void reset() override;

    QString createResultPath() override;

private:
    qreal mean(const GA::Types::GenomePopulation &population);
    qreal stdDev(const GA::Types::GenomePopulation &population, const qreal avg);
    GA::Types::Points min(const GA::Types::GenomePopulation &population);
};
