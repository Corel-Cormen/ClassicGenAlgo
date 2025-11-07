#pragma once

#include "FunctionInterface.hpp"

class BC_1 : public FunctionInterface
{
public:
    BC_1() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"BC 1"};
    static constexpr QStringView crateFuncName{u"make_bc_1"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
