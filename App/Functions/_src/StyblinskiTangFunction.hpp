#pragma once

#include "FunctionInterface.hpp"

class StyblinskiTangFunction : public FunctionInterface
{
public:
    StyblinskiTangFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Styblinski-Tang"};
    static constexpr QStringView crateFuncName{u"make_styblinski_tang"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
