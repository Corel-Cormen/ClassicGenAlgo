#pragma once

#include <QStringView>

#include "PyInterface.hpp"

class PyQt final : public PyInterface
{
public:

    bool startPython() override;

    void stopPython() override;

    bool makeFunction(const QString& funcName, const size_t dimension) override;

    void discardFunction() override;

    bool calcPoint(const std::vector<qreal>& point, qreal &val) override;

    void showPlot(const GA::Types::Points &point,
                  const QString &savePath,
                  const bool showFlag) override;

    void showStats(const std::vector<GA::Types::Points> &bestValues,
                   const std::vector<qreal> avgValues,
                   const std::vector<qreal> stdValues,
                   const QString &saveBestPlotPath,
                   const QString &saveBestDataPath,
                   const QString &saveStatPlotPath,
                   const QString &saveStatDataPath,
                   const bool showFlag) override;

private:
    static constexpr QStringView PyMainPath{u"../python"};
    static constexpr QStringView PyModulePath{u"_py"};

    static constexpr QStringView PyFunctionFabricModuleName{u"benchmark_funcs"};
    static constexpr QStringView PyFunctionCallerModuleName{u"function_caller"};
    static constexpr QStringView PyFunctionVisualizationModuleName{u"visualization"};
    static constexpr QStringView PyFunctionVisualizationChartsModuleName{u"plotting_fitness"};

    static constexpr QStringView PyCalcFunctionName{u"call_func"};
    static constexpr QStringView PyShowCharFunctionName{u"show_char"};
    static constexpr QStringView PyShowCharSummaryName{u"plot_best_fitness_history"};
    static constexpr QStringView PyShowCharFitnessHistoryName{u"plot_avg_fitness_history"};

    static bool pyInitConfig();
    static bool addModulePath();
    static bool createFileHandlers();
    static bool createFunctionHandlers();

    static void plotBestValues(const std::vector<GA::Types::Points> &bestValues,
                               const QString &saveBestPlotPath,
                               const QString &saveBestDataPath,
                               const bool showFlag);
    static void plotStatsValues(const std::vector<qreal> avgValues,
                                const std::vector<qreal> stdValues,
                                const QString &saveStatPlotPath,
                                const QString &saveStatDataPath,
                                const bool showFlag);
};
