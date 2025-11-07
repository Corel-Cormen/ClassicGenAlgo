#pragma once

#include "FunctionInterface.hpp"

class RosenbrockFunction : public FunctionInterface
{
public:
    RosenbrockFunction() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"RosenbrockFunction"};
    static constexpr QStringView crateFuncName{u"make_rosenbrock"};
    static constexpr FunctionType functionType{FunctionType::BENCHMARK_FUNC};
};
