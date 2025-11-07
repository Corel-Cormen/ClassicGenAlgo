#pragma once

#include "FunctionInterface.hpp"

class BC_35 : public FunctionInterface
{
public:
    BC_35() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"BC 35"};
    static constexpr QStringView crateFuncName{u"make_bc_35"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
