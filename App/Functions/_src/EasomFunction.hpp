#pragma once

#include "FunctionInterface.hpp"

class EasomFunction : public FunctionInterface
{
public:
    EasomFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Easom"};
    static constexpr QStringView crateFuncName{u"make_easom"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
