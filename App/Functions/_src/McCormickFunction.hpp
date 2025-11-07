#pragma once

#include "FunctionInterface.hpp"

class McCormickFunction : public FunctionInterface
{
public:
    McCormickFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Mc Cormick"};
    static constexpr QStringView crateFuncName{u"make_mc_cormick"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
