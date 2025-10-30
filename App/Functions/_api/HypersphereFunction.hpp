#pragma once

#include "FunctionInterface.hpp"

class HypersphereFunction : public FunctionInterface
{
public:
    HypersphereFunction() :
        FunctionInterface(funcName, crateFuncName)
    {}

private:

    static constexpr QStringView funcName{u"Hypersphere"};
    static constexpr QStringView crateFuncName{u"make_hypersphere"};
};
