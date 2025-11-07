#pragma once

#include "FunctionInterface.hpp"

class AckleyFunction : public FunctionInterface
{
public:
    AckleyFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Ackley"};
    static constexpr QStringView crateFuncName{u"make_ackley"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
