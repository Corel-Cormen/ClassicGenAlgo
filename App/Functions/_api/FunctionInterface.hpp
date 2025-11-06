#pragma once

#include <QStringView>

enum class FunctionType : uint8_t
{
    BENCHMARK_FUNC = 0,
    CEC_FUNC = 1,
};

class FunctionInterface
{
public:
    FunctionInterface() = delete;

    explicit FunctionInterface(QStringView funcName,
                               QStringView crateFuncName,
                               FunctionType functionType) :
        funcName{funcName},
        crateFuncName{crateFuncName},
        functionType{functionType}
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

    inline FunctionType getType() const
    {
        return functionType;
    }

private:
    const QStringView funcName;
    const QStringView crateFuncName;
    const FunctionType functionType;
};
