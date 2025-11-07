#pragma once

#include <vector>

#include <QString>
#include <QtTypes>

#include "GeneticAlgorithmTypes.hpp"

class PyInterface
{
public:

    virtual ~PyInterface() = default;

    virtual bool startPython() = 0;

    virtual void stopPython() = 0;

    virtual bool makeFunction(const QString& funcName, const size_t dimension) = 0;

    virtual void discardFunction() = 0;

    virtual void showPlot(const GA::Types::Points &point,
                          const QString &savePath,
                          const bool showFlag) = 0;

    virtual void showStats(const std::vector<GA::Types::Points> &bestValues,
                           const std::vector<qreal> avgValues,
                           const std::vector<qreal> stdValues,
                           const QString &saveBestPlotPath,
                           const QString &saveBestDataPath,
                           const QString &saveStatPlotPath,
                           const QString &saveStatDataPath,
                           const bool showFlag) = 0;

    [[nodiscard]]
    virtual bool calcPoint(const std::vector<qreal>& point, qreal &val) = 0;
};
