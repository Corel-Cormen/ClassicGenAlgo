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

private:
    static constexpr QStringView PyModulePath{u"_py"};
    static constexpr QStringView PyModuleName{u"functions"};
    static constexpr QStringView PyCalcFunctionName{u"call_func"};
};
