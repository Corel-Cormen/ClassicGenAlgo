#pragma once

#include "FunctionInterface.hpp"

class BC_2 : public FunctionInterface
{
public:
    BC_2() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"BC 2"};
    static constexpr QStringView crateFuncName{u"make_bc_2"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
