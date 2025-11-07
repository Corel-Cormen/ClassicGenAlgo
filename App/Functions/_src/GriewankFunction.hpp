#pragma once

#include "FunctionInterface.hpp"

class GriewankFunction : public FunctionInterface
{
public:
    GriewankFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Griewank"};
    static constexpr QStringView crateFuncName{u"make_griewank"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
