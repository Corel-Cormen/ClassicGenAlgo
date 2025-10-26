#pragma once

#include <QStringView>

class FunctionInterface
{
public:
    explicit FunctionInterface(QStringView funcName,
                               QStringView crateFuncName) :
        funcName{funcName},
        crateFuncName{crateFuncName}
    {}

    virtual ~FunctionInterface() = default;

    inline QStringView getName() const
    {
        return funcName;
    }

    inline QStringView getCreateFuncName() const
    {
        return crateFuncName;
    }

protected:
    QStringView funcName;
    QStringView crateFuncName;
};
