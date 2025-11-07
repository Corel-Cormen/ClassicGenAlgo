#pragma once

#include "FunctionInterface.hpp"

class DeJong5Function : public FunctionInterface
{
public:
    DeJong5Function() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"De Jong 5"};
    static constexpr QStringView crateFuncName{u"make_de_jong_5"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
