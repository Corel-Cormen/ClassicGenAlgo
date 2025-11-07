#pragma once

#include "FunctionInterface.hpp"

class GoldsteinAndPrice : public FunctionInterface
{
public:
    GoldsteinAndPrice() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Goldstein and Price"};
    static constexpr QStringView crateFuncName{u"make_goldstein_and_price"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
