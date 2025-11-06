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

    void showPlot(const GA::Types::GenomePoint &point) override;

private:
    static constexpr QStringView PyMainPath{u"../python"};
    static constexpr QStringView PyModulePath{u"_py"};

    static constexpr QStringView PyFunctionFabricModuleName{u"benchmark_funcs"};
    static constexpr QStringView PyFunctionCallerModuleName{u"function_caller"};
    static constexpr QStringView PyFunctionVisualizationModuleName{u"visualization"};

    static constexpr QStringView PyCalcFunctionName{u"call_func"};
    static constexpr QStringView PyShowCharFunctionName{u"show_char"};

    static bool pyInitConfig();
    static bool addModulePath();
    static bool createFileHandlers();
    static bool createFunctionHandlers();
};
