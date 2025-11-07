#pragma once

#include "FunctionInterface.hpp"

class MichalewiczFunction : public FunctionInterface
{
public:
    MichalewiczFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Michalewicz"};
    static constexpr QStringView crateFuncName{u"make_michalewicz"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
