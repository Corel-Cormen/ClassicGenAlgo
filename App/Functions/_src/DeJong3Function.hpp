#pragma once

#include "FunctionInterface.hpp"

class DeJong3Function : public FunctionInterface
{
public:
    DeJong3Function() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"De Jong 3"};
    static constexpr QStringView crateFuncName{u"make_de_jong_3"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
