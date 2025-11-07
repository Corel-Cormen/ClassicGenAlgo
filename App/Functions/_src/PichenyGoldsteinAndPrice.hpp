#pragma once

#include "FunctionInterface.hpp"

class PichenyGoldsteinAndPrice : public FunctionInterface
{
public:
    PichenyGoldsteinAndPrice() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Picheny Goldstein and Price"};
    static constexpr QStringView crateFuncName{u"make_picheny_goldstein_and_price"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
