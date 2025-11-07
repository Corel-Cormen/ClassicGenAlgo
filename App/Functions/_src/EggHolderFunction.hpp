#pragma once

#include "FunctionInterface.hpp"

class EggHolderFunction : public FunctionInterface
{
public:
    EggHolderFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Egg Holder"};
    static constexpr QStringView crateFuncName{u"make_egg_holder"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
