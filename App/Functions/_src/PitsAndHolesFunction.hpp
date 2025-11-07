#pragma once

#include "FunctionInterface.hpp"

class PitsAndHolesFunction : public FunctionInterface
{
public:
    PitsAndHolesFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Pits and Holes"};
    static constexpr QStringView crateFuncName{u"make_pits_and_holes"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
