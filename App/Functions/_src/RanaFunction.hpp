#pragma once

#include "FunctionInterface.hpp"

class RanaFunction : public FunctionInterface
{
public:
    RanaFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Rana"};
    static constexpr QStringView crateFuncName{u"make_rana"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
