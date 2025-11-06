#pragma once

#include "FunctionInterface.hpp"

class HypersphereFunction : public FunctionInterface
{
public:
    HypersphereFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Hypersphere"};
    static constexpr QStringView crateFuncName{u"make_hypersphere"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
