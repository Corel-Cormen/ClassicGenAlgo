#pragma once

#include "FunctionInterface.hpp"

class HypersphereFunction : public FunctionInterface
{
public:

    inline QStringView name() const override
    {
        return funName;
    }

private:

    static constexpr QStringView funName{u"Hypersphere"};
};
