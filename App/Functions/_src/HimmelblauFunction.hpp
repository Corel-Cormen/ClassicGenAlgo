#pragma once

#include "FunctionInterface.hpp"

class HimmelblauFunction : public FunctionInterface
{
public:
    HimmelblauFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Himmelblau"};
    static constexpr QStringView crateFuncName{u"make_Himmelblau"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
