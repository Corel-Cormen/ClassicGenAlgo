#pragma once

#include <vector>

#include <QString>
#include <QtTypes>

class PyInterface
{
public:

    virtual ~PyInterface() = default;

    virtual bool startPython() = 0;

    virtual void stopPython() = 0;

    virtual bool makeFunction(const QString& funcName, const size_t dimension) = 0;

    virtual void discardFunction() = 0;

    [[nodiscard]]
    virtual bool calcPoint(const std::vector<qreal>& point, qreal &val) = 0;
};
