#pragma once

#include "FunctionInterface.hpp"

class GriewankCEC2014Function : public FunctionInterface
{
public:
    GriewankCEC2014Function() :
        FunctionInterface(funcName, crateFuncName, functionType)
    {}

private:

    static constexpr QStringView funcName{u"Griewank CEC2014"};
    static constexpr QStringView crateFuncName{u"make_griewank_cec2014"};
    static constexpr FunctionType functionType{FunctionType::CEC_FUNC};
};
