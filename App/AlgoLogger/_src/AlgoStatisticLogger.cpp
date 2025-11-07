#include <algorithm>
#include <cmath>
#include <numeric>

#include <QDir>
#include <QDateTime>

#include "AlgoStatisticLogger.hpp"
#include "CommonFunctions.hpp"

void AlgoStatisticLogger::colectData(const GA::Types::GenomePopulation &population)
{
    if (!population.empty())
    {
        const qreal avg = mean(population);
        meanVec.push_back(avg);
        stdDevVec.push_back(stdDev(population, avg));
        minVec.push_back(min(population));
    }
}

qreal AlgoStatisticLogger::mean(const GA::Types::GenomePopulation &population)
{
    qreal sum = std::accumulate(population.begin(), population.end(), 0.0,
                                [](qreal acc, const GA::Types::GenomePoint& gp) {
                                    return acc + gp.value;
                                });
    return (sum / population.size());
}

qreal AlgoStatisticLogger::stdDev(const GA::Types::GenomePopulation &population, const qreal avg)
{
    qreal sumSquare = std::accumulate(population.begin(), population.end(), 0.0,
                                      [avg](qreal acc, const GA::Types::GenomePoint& gp) {
                                            qreal diff = gp.value - avg;
                                            return acc + diff * diff;
                                        });
    return std::sqrt(sumSquare / population.size());
}

GA::Types::Points AlgoStatisticLogger::min(const GA::Types::GenomePopulation &population)
{
    auto it = std::min_element(population.begin(), population.end(),
                               [](const GA::Types::GenomePoint& a, const GA::Types::GenomePoint& b) {
                                   return CommonFunc::lessThan(a.value, b.value);
                               });
    GA::Types::Points result;
    result.point.reserve(it->point.size());
    for (const auto &val : it->point)
    {
        result.point.push_back(val.val());
    }
    result.value = it->value;
    return result;
}

void AlgoStatisticLogger::reset()
{
    stdDevVec.clear();
    meanVec.clear();
    minVec.clear();
}

QString AlgoStatisticLogger::createResultPath()
{
    QString basePath = "results";
    QString dateTimeFolder = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString fullPath = basePath + "/" + dateTimeFolder;

    QDir dir;
    dir.mkpath(fullPath);

    return fullPath;
}
