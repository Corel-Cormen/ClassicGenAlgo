#pragma once

#include "FunctionInterface.hpp"

class SchwefelFunction : public FunctionInterface
{
public:
    SchwefelFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Schwefel"};
    static constexpr QStringView crateFuncName{u"make_schwefel"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
