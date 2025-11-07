#pragma once

#include "FunctionInterface.hpp"

class HyperellipsoidFunction : public FunctionInterface
{
public:
    HyperellipsoidFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Hyperellipsoid"};
    static constexpr QStringView crateFuncName{u"make_hyperellipsoid"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
