#pragma once

#include "FunctionInterface.hpp"

class KeaneFunction : public FunctionInterface
{
public:
    KeaneFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Keane"};
    static constexpr QStringView crateFuncName{u"make_keane"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
