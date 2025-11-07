#pragma once

#include "FunctionInterface.hpp"

class RastriginFunction : public FunctionInterface
{
public:
    RastriginFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Rastrigin"};
    static constexpr QStringView crateFuncName{u"make_rastrigin"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
