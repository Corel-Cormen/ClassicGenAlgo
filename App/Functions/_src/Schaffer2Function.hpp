#pragma once

#include "FunctionInterface.hpp"

class Schaffer2Function : public FunctionInterface
{
public:
    Schaffer2Function() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Schaffer 2"};
    static constexpr QStringView crateFuncName{u"make_schaffer_2"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
